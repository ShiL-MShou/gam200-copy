/************************************************************************************ /
/*
File Name:		ParticlesManager.h
Project Name:	WhereGotLate
Author(s):		Tay Qin Wen Alvin [100%]


Brief:			Manages all the emiiter.


Copyright Information:
All content © 2019 DigiPen (SINGAPORE) Corporation, all rights reserved.
*/
/*************************************************************************************/


#pragma once

#include <list>
#include <memory>
#include "Emitter.h"
#include "Transform.h"
#include "Singleton.h"

#define MyParticleManager (ParticleManager::GetInstance())


class ParticleManager : public Singleton<ParticleManager>
{
	std::list<std::shared_ptr<Emitter>> emitterList;

	ParticleManager();
	~ParticleManager();

	ParticleManager(const ParticleManager&) = delete;
	ParticleManager& operator=(const ParticleManager&) = delete;

	friend class Singleton<ParticleManager>;


public:
	void Initialize();

	void Update(float _dt);

	void Destroy();
	void Clear();

	// --- create Emitter ---
	void CreateEmitter(	EmitterType _type, VEC2 position, float _startDelay, float _Lifetime = 10.0f, float _particleLifetime = 2.0f, float _fadeSpeed = 2.0f, float _emitInterval = 1.25f,
						float _emitNum = 5, VEC2 _emitArea = VEC2(5.0f, 5.0f), float _angle = 0.0f, float _movementSpeed = 5.0f, std::string _archetype = "Particles");
	
	void CreateEmitter(	EmitterType _type, std::shared_ptr<Transform> _transformPtr, VEC2 _offset, float _startDelay, float _lifetime = 2.0f, float _particleLifetime = 2.0f, float _fadeSpeed = 2.0f, float _emitInterval = 0.25f,
						float _emitNum = 5, VEC2 _emitArea = VEC2(5.0f, 5.0f), float _angle = 0.0f, float _movementSpeed = 5.0f, std::string _archetype = "Particles");

};

