#ifndef _PROJECTILE_H_
#define _PROJECTILE_H_

#include "fv2.h"
#include "Sprite.h"
#include "ParticleSystem.h"

typedef struct Projectile
{	
	// Projectile Data
	DynSprite Sprite; // Dynamic Sprite, holds position, velocity, size and OAMAttributes
	u8 iDamage; // How much damage this projectile will apply on collision
	u16 iSpeed; // How fast the bullet moves (Speed * Velocity)
	bool bActive; // To test if this projectile is active for iterations and collision testing
	u8 AnimationCounter;
	bool bHostile;

	// Particles
	ObjectAttribute ParticleAttributes; // Stores the particles attributes (Stored here as all particles will be the same, saves memory)
	Emitter ParticleEmitter; // Stores the Emitters location
	Particle ParticleArray[4]; // Stores all particles that this projectile can use
	u8 particleCount; // Used to test if particles can be emitted for iterations
}Projectile;

extern void SetProjectileSprite(Projectile* a_projectile, u16 attr0, u16 attr1, u16 attr2); // Ease of use to set OBJAttribs
extern void InitialiseProjectile(Projectile* a_projectile, u8 a_damage, u16 a_speed, bool a_bHostile); // Initialises the projectiles called by the owning entity (Player, Enemy)
extern void UpdateProjectile(Projectile* a_projectile); // Runs at the end of entity update to update their projectiles
extern void SetDamage(Projectile* a_projectile, u8 a_damage); // Sets the damage of the projectiles
extern void Fire(Projectile* a_projectile, fv2 a_startPos, fv2 a_direction); // Activates a projectile and gives is the data for UpdateProjectiles to manipulate

#endif // !_PROJECTILE_H_
