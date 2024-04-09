/************************************************************************************ /
/*
File Name:		ParticlesManager.cpp
Project Name:	WhereGotLate
Author(s):		Tay Qin Wen Alvin [100%]


Brief:			Manages all the emiiter.


Copyright Information:
All content © 2019 DigiPen (SINGAPORE) Corporation, all rights reserved.
*/
/*************************************************************************************/

#include "stdafx.h"
#include "ParticleManager.h"

#include "EmitterDynamic.h"
#include "EmitterStatic.h"

#include "Factory.h"


ParticleManager::ParticleManager()
{
}

ParticleManager::~ParticleManager()
{
}

void ParticleManager::Initialize()
{
	FACTORY->CreateArcheType("Particles");
	FACTORY->CreateArcheType("SweatDrops");
}

void ParticleManager::Update(float _dt)
{
	UNREFERENCED_PARAMETER(_dt);

	for (auto itr = std::begin(emitterList); itr != std::end(emitterList); )
	{
		if ((*itr)->GetEmitterType() == EmitterType::DYNAMIC)
		{
			std::shared_ptr<EmitterDynamic> emitterDynamicPtr = std::dynamic_pointer_cast<EmitterDynamic>(*itr);
			emitterDynamicPtr->Update(_dt);
		}
		else  // static and static random
		{
			std::shared_ptr<EmitterStatic> emitterStaticPtr = std::dynamic_pointer_cast<EmitterStatic>(*itr);
			emitterStaticPtr->Update(_dt);
		}

		(*itr)->GetParticlesCount();

		if ((*itr)->IsActive() == false && (*itr)->GetParticlesCount() == 0)
		{
			emitterList.erase(itr++);
		}
		else
		{
			++itr;
		}
	}

}

void ParticleManager::Destroy()
{
}

void ParticleManager::Clear()
{
	for (auto emitter : emitterList)
		emitter->Destroy();
	emitterList.clear();
}

void ParticleManager::CreateEmitter(EmitterType _type, VEC2 _position, float _startDelay, float _lifetime, float _particleLifetime, float _fadeSpeed, float _emitInterval, float _emitNum, VEC2 _emitArea, float _angle, float _movementSpeed, std::string _archetype)
{
	std::shared_ptr<Emitter> emitterPtr;

	if (_type == EmitterType::DYNAMIC)
		;// emitterPtr = std::make_shared<EmitterDynamic>(std::make_shared<Transform>());
	else
		emitterPtr = std::make_shared<EmitterStatic>(_type, _position, _startDelay, _lifetime, _particleLifetime, _fadeSpeed, _emitInterval, _emitNum, _emitArea, _angle, _movementSpeed, _archetype);

	emitterList.push_back(emitterPtr);
}

void ParticleManager::CreateEmitter(EmitterType _type, std::shared_ptr<Transform> _transformPtr, VEC2 _offset, float _startDelay, float _lifetime, float _particleLifetime, float _fadeSpeed, float _emitInterval, float _emitNum, VEC2 _emitArea, float _angle, float _movementSpeed, std::string _archetype)
{
	std::shared_ptr<Emitter> emitterPtr;

	if (_type == EmitterType::DYNAMIC)
		emitterPtr = std::make_shared<EmitterDynamic>(_transformPtr, _type, _offset, _startDelay, _lifetime, _particleLifetime, _fadeSpeed, _emitInterval, _emitNum, _emitArea, _angle, _movementSpeed, _archetype);

	emitterList.push_back(emitterPtr);
}
