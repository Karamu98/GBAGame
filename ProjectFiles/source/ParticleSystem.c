#include "ParticleSystem.h"
#include "string.h"
#include "MemoryManager.h"

#include "../images/EmberParticle.h"

void InitEmitter(Emitter * a_e, fv2 * a_pos, bool a_bIsCone, fv2 * a_forward)
{
	a_e->bIsCone = a_bIsCone;
	a_e->vPosition = *(a_pos);
	a_e->vForward = *(a_forward);
}

void InitParticle(Particle* a_p, u16 a_minLife, u16 a_maxLife, u16 a_minSpeed, u16 a_maxSpeed, fv2 a_minVel, fv2 a_maxVel)
{
	a_p->Sprite.vPosition = fv2Create( 0, 0 ); // Zero
	a_p->Sprite.vPosition = fv2Create( 0, 0 ); // Zero
	a_p->age = 0; // Zero
	a_p->bAlive = false;

	a_p->minLife = a_minLife;
	a_p->maxLife = a_maxLife;
	a_p->minSpeed = a_minSpeed;
	a_p->maxSpeed = a_maxSpeed;
	a_p->minVelocity = a_minVel;
	a_p->maxVelocity = a_maxVel;

	// Set sprite attribs
	SetParticleSprite
	(
		a_p,
		SetAttrib0(fix2int(a_p->Sprite.vPosition.fY), 0, 0, 0, 0, A0_SQUARE),
		SetAttrib1(fix2int(a_p->Sprite.vPosition.fX), 0, A1_SIZE_1),
		SetAttrib2(1020, 0, 3)
	);

	obj_hide(&a_p->Sprite.Attributes);
}



void EmitParticle(Emitter* a_e, Particle* a_p)
{
	if (a_p->bAlive == false)
	{
		a_p->Sprite.SpriteID = GetFreeSprite();
		TagSprite(a_p->Sprite.SpriteID);


		obj_unhide(&a_p->Sprite.Attributes, 0);
		a_p->age = 0; // Reset it's life span
		a_p->Sprite.vPosition = a_e->vPosition; // Move the particle to the emitters location
		a_p->bAlive = true; // Change the state of the particle to active

		a_p->lifeSpan = (qran_range(a_p->minLife, a_p->maxLife)); // Give a random life span
		a_p->iSpeed = (qran_range(a_p->minSpeed, a_p->maxSpeed)); // Give a random speed


		if (a_e->bIsCone)
		{
			// Give the particle a random velocity in range ((-1, -1) to (1, 1) modified with direction for cone shape
			a_p->Sprite.vVelocity.fX = int2fix(qran_range(-2, 2)); // Ask why -1 to 1 doesnt work
			a_p->Sprite.vVelocity.fY = int2fix(qran_range(-2, 2));


			a_p->Sprite.vVelocity = fvAdd(a_p->Sprite.vVelocity, a_e->vForward);

		}
		else
		{
			// Get a random velocity
			a_p->Sprite.vVelocity.fX = int2fix(qran_range(-2, 2)); // Ask why -1 to 1 doesnt work
			a_p->Sprite.vVelocity.fY = int2fix(qran_range(-2, 2));
		}
	}
}

int counter = 0;

void UpdateParticle(Particle* a_p)
{
	if (a_p->bAlive == true)
	{
		a_p->age += 1; // Increment the particles life time
		if (a_p->age < a_p->lifeSpan)
		{
			fv2 temp = fv2Create(a_p->iSpeed, a_p->iSpeed); // Temp to allow for fv2 use
			a_p->Sprite.vPosition = fvAdd(a_p->Sprite.vPosition, fvMul(a_p->Sprite.vVelocity, temp)); // Move the particle based on its velocity

			SetObjectvPosition(&a_p->Sprite.Attributes, a_p->Sprite.vPosition); // Change Attribs with new pos


			MEM_OAM[a_p->Sprite.SpriteID] = a_p->Sprite.Attributes; // Apply to OAM
		}
		else
		{
			a_p->bAlive = false; // Deactivate the particle
			DropSprite(a_p->Sprite.SpriteID); // Free the sprite
			obj_hide(&a_p->Sprite.Attributes); // Hide the sprite

			MEM_OAM[a_p->Sprite.SpriteID] = a_p->Sprite.Attributes; // Apply to OAM
		}

	}
	else
	{

	}

	counter++;
	if (counter > 256)
	{
		counter = 0;
	}
	if (counter % 32 == 0)
	{
		memcpy(sprite_tile_block_address(1020), &EmberParticleTiles[EmberFrameSize * (counter >> 5)], EmberFrameLen);
	}
}

void SetParticleSprite(Particle * a_particle, u16 attr0, u16 attr1, u16 attr2)
{
	a_particle->Sprite.Attributes.attr0 = attr0;
	a_particle->Sprite.Attributes.attr1 = attr1;
	a_particle->Sprite.Attributes.attr2 = attr2;
}
