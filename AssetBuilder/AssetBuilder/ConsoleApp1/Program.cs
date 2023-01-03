using System;
using System.Xml;
using System.Configuration;
using System.Diagnostics;
using TiledCS;

namespace AssetBuilder
{
    internal class Program
    {
        static void Main(string[] args)
        {
            string binPath = ConfigurationManager.AppSettings.Get("Tiled2GBA");
            string mapsSrc = ConfigurationManager.AppSettings.Get("MapsSrc");
            string mapsDst = ConfigurationManager.AppSettings.Get("MapsDst");
            bool clearDst = ConfigurationManager.AppSettings.Get("ClearDst").Equals("true", StringComparison.OrdinalIgnoreCase);

            if(clearDst)
            {
                Directory.Delete(mapsDst, true);
                Directory.CreateDirectory(mapsDst);
            }

            foreach(var dir in Directory.GetDirectories(mapsSrc))
            {
                string[] files = Directory.GetFiles(dir, "*.tmx");
                if(files.Length > 0)
                {
                    string mapName = Path.GetFileNameWithoutExtension(files[0]);
                    string mapDestPath = $"{Path.GetFullPath(mapsDst)}{mapName}";

                    string command = $"{Path.GetFullPath(binPath)} {Path.GetFullPath(files[0])} {mapDestPath}.c {mapDestPath}.h";
                    ExecuteCommand(command);
                }
            }
        }

        private static void TiledToGBA(string tmxPath)
        {
            TiledMap map = new TiledMap(tmxPath);
            
            // Only allow valid map sizes to be parsed
            if (map.Width % 32 != 0 || map.Height % 32 != 0)
            {
                Console.WriteLine("Export failed: Invalid map size! Map width and height must be a multiple of 32.");
                return;
            }

            // Standard screenblock size for GBA
            const int SCREENBLOCKWIDTH = 32;
            const int SCREENBLOCKHEIGHT = 32;

            // Split full filename path into the filename (without extension) and the directory
            var fileBaseName = Path.GetFileNameWithoutExtension(tmxPath);
            var filePath = Path.GetDirectoryName(Path.GetFullPath(tmxPath));

            var tilemapLength = map.Width * map.Height;

            var headerFileData = "";
            headerFileData += "#ifndef " + fileBaseName.ToUpper() + "_H\n";
            headerFileData += "#define " + fileBaseName.ToUpper() + "_H\n\n";
            headerFileData += "#define " + fileBaseName.ToUpper() + "_WIDTH  (" + map.Width + ")\n";
            headerFileData += "#define " + fileBaseName.ToUpper() + "_HEIGHT (" + map.Height + ")\n";
            headerFileData += "#define " + fileBaseName.ToUpper() + "_LENGTH (" + tilemapLength + ")\n\n";

            var sourceFileData = "";
            sourceFileData += "#include \"" + fileBaseName + ".h\"\n\n";

            for (int i = 0; i < map.Layers.Count(); ++i)
            {
                TiledLayer currentLayer = map.Layers[i];

                // Replace special characters for an underscore
                string currentLayerName = currentLayer.name.replace(/[^a - zA - Z0 - 9 - _] / g, "_");

                headerFileData += "extern const unsigned short " + currentLayerName + "[" + tilemapLength + "];\n";

                sourceFileData += "const unsigned short " + currentLayerName + "[" + tilemapLength + "] __attribute__((aligned(4))) =\n";
                sourceFileData += "{\n";

                int screenBlockCountX = currentLayer.width / SCREENBLOCKWIDTH;
                int screenBlockCountY = currentLayer.height / SCREENBLOCKHEIGHT;
                int screenBlockID = 0;

                if (currentLayer.type == TiledLayerType.TileLayer)
                {
                    for (int j = 0; j < screenBlockCountY; ++j)
                    {
                        for (int k = 0; k < screenBlockCountX; ++k)
                        {
                            sourceFileData += "    // Screenblock " + screenBlockID + "\n";
                            screenBlockID++;

                            for (int y = 0; y < SCREENBLOCKHEIGHT; ++y)
                            {
                                // Indent array rows
                                sourceFileData += "    ";

                                for (int x = 0; x < SCREENBLOCKWIDTH; ++x)
                                {
                                    int currentTileX = x + (SCREENBLOCKWIDTH * k);
                                    int currentTileY = y + (SCREENBLOCKHEIGHT * j);
                                    int currentTile = currentLayer.data[(currentTileY * currentLayer.width) + currentTileX];
                                    var currentTileID = currentTile.tileId;

                                    // Default to 0x0000 for blank tiles
                                    if (currentTileID == "-1")
                                    {
                                        sourceFileData += "0x0000, ";
                                    }
                                    else
                                    {
                                        if (currentTile.flippedHorizontally)
                                        {
                                            // Set the HFLIP bit for this screen entry
                                            currentTileID |= (1 << 10);
                                        }
                                        if (currentTile.flippedVertically)
                                        {
                                            // Set the VFLIP bit for this screen entry
                                            currentTileID |= (1 << 11);
                                        }

                                        sourceFileData += decimalToHex(currentTileID, 4) + ", ";
                                    }
                                }

                                sourceFileData += "\n";
                            }

                            sourceFileData += "\n";
                        }
                    }
                }

                // Remove the last comma and close the array.
                sourceFileData = sourceFileData.slice(0, -3) + "\n};\n\n";
            }

            headerFileData += "\n#endif\n";

            // Remove the second newline at the end of the source file
            sourceFileData = sourceFileData.slice(0, -1);

            // Write header data to disk
            var headerFile = new TextFile(filePath + fileBaseName + ".h", TextFile.WriteOnly);
            headerFile.write(headerFileData);
            headerFile.commit();
            console.log("Tilemap exported to " + filePath + fileBaseName + ".h");

            // Write source data to disk
            var sourceFile = new TextFile(filePath + fileBaseName + ".c", TextFile.WriteOnly);
            sourceFile.write(sourceFileData);
            sourceFile.commit();
            console.log("Tilemap exported to " + filePath + fileBaseName + ".c");

            console.timeEnd("Export completed in");
        }
    }

        private static void ExecuteCommand(string command)
        {
            try
            {
                ProcessStartInfo procStartInfo = new ProcessStartInfo("cmd", "/c " + command);

                procStartInfo.RedirectStandardError = true;
                procStartInfo.RedirectStandardOutput = true;
                procStartInfo.UseShellExecute = false;
                procStartInfo.CreateNoWindow = true;

                Process proc = new Process();
                proc.StartInfo = procStartInfo;
                proc.Start();

                string errors = proc.StandardError.ReadToEnd();
                string log = proc.StandardOutput.ReadToEnd();

                Console.WriteLine(errors);
                Console.WriteLine(log);
            }
            catch (Exception objException)
            {
                Console.WriteLine(objException.Message);
            }
        }
    }
}