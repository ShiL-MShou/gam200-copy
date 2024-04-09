/************************************************************************************ /
/*
File Name:		Emitter.cpp
Project Name:	WhereGotLate
Author(s):		Tay Qin Wen Alvin [100%]


Brief:			Base for all the emitters.


Copyright Information:
All content © 2019 DigiPen (SINGAPORE) Corporation, all rights reserved.
*/
/*************************************************************************************/


#include "stdafx.h"
#include "Emitter.h"

// to create the object
#include "Logic.h"

Emitter::Emitter()
{
}

Emitter::Emitter(EmitterType _type, float _startDelay, float _lifetime, float _particleLifetime, float _fadeSpeed, float _emitInterval, float _emitNum, VEC2 _emitArea, std::string _archetype)
	: type{ _type }, startDelay{ _startDelay }, lifetime{ _lifetime }, particleLifetime{ _particleLifetime }, fadeSpeed{ _fadeSpeed }, 
	emitInterval{ _emitInterval }, emitNum{ _emitNum }, emitArea{ _emitArea }, isActive{ true }, isPaused{ false }, 
	archeTypeName{ _archetype }, particleList{}, intervalCounter{0}
{
}

Emitter::~Emitter()
{
}

void Emitter::CreateParticles()
{
}

void Emitter::Update(float _dt)
{
	UNREFERENCED_PARAMETER(_dt);
}

void Emitter::Destroy()
{
	particleList.clear();
}

bool Emitter::IsActive()
{
	return isActive;
}

EmitterType Emitter::GetEmitterType()
{
	return type;
}

int Emitter::GetParticlesCount()
{
	return static_cast<int>(particleList.size());
}

void Emitter::SetParticleArcheType(std::string _name)
{
	archeTypeName = _name;
}
