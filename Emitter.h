/************************************************************************************ /
/*
File Name:		Emitter.h
Project Name:	WhereGotLate
Author(s):		Tay Qin Wen Alvin [100%]


Brief:			Base for all the emitters.


Copyright Information:
All content © 2019 DigiPen (SINGAPORE) Corporation, all rights reserved.
*/
/*************************************************************************************/


#pragma once

#include <list>
#include <memory>

#include "SpriteComponent.h"
#include "Vector2.h"

#define EMITTER_INFINITE_LIFETIME 10000.0f

enum class EmitterType
{
	STATIC = 0,
	STATIC_RANDOM,		// random animation
	DYNAMIC
};

class Emitter
{

protected:
	std::list<std::pair<float, std::shared_ptr<SpriteComponent>>> particleList;

	EmitterType type;

	bool isPaused;
	bool isActive;

	float startDelay;
	float lifetime;
	float particleLifetime;
	float fadeSpeed;			// not used
	float emitInterval;
	float emitNum;

	float intervalCounter;

	VEC2 emitArea;

	std::string archeTypeName;


public:
	Emitter();
	Emitter(EmitterType _type, float _startDelay, float _lifetime, float _particleLifetime, float _fadeSpeed, float _emitInterval, float _emitNum, VEC2 _emitArea, std::string _archetype);

	virtual ~Emitter();


	virtual void CreateParticles();
	 
	virtual void Update(float _dt);

	virtual void Destroy();

	bool IsActive();
	EmitterType GetEmitterType();

	int GetParticlesCount();

	void SetParticleArcheType(std::string _name);
};

