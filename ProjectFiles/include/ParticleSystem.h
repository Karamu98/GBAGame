#ifndef _PARTICLESYSTEM_H_
#define _PARTICLESYSTEM_H_

#include "gba_types.h"
#include "gba_math.h"
#include "gba_mathUtil.h"
#include "gba_gfx.h"
#include "fv2.h"
#include "Sprite.h"


typedef struct Emitter
{
	bool bIsCone; // For emitting particles at random
	fv2 vForward; // To be applied to random velocity to "cone" emission

	fv2 vPosition; // Stores position of emitter
}Emitter;

typedef struct Particle
{
	// Visual
	DynSprite Sprite; // Dynamic Sprite, holds position, velocity, size and OAMAttributes

	// Speed
	u16 iSpeed; // How fast the particle moves (Speed * Velocity)
	u16 minSpeed; // Slowest the particle will move at
	u16 maxSpeed; // Max speed for the particle to move at

	// Direction
	fv2 minVelocity;
	fv2 maxVelocity;

	// Life
	u8 age; // Counter for the particles to be deactivated
	u8 lifeSpan; // Stores the life of each particles
	u8 minLife; // Minimum the lifespan can be
	u8 maxLife; // Maximum age the particle can reach

	// Tests
	bool bAlive; // For update to test if this particle needs to be updated or if this can be emitted
}Particle;

extern void InitEmitter(Emitter* a_e, fv2* a_pos, bool a_bIsCone, fv2* a_forward); // Initialises the emitter for particles to use (If not cone, radial)

extern void InitParticle(Particle* a_p, u16 a_minLife, u16 a_maxLife, u16 a_minSpeed, u16 a_maxSpeed, fv2 a_minVel, fv2 a_maxVel); // Intiates the particle, called on owner initialisation
extern void EmitParticle(Emitter* a_e, Particle* a_p); // Called when wanting to emit a particle, called on owner
extern void UpdateParticle(Particle* a_p); // Updates the particles, called on owner

extern void SetParticleSprite(Particle* a_particle, u16 attr0, u16 attr1, u16 attr2); // Ease of use to set particle sprite

#endif
