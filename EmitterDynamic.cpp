/************************************************************************************ /
/*
File Name:		EmitterDynamic.cpp
Project Name:	WhereGotLate
Author(s):		Tay Qin Wen Alvin [100%]


Brief:			Dynamic emitter.


Copyright Information:
All content © 2019 DigiPen (SINGAPORE) Corporation, all rights reserved.
*/
/*************************************************************************************/

#include "stdafx.h"
#include "EmitterDynamic.h"

#include "Logic.h"


EmitterDynamic::EmitterDynamic()
{
}

EmitterDynamic::EmitterDynamic(	std::shared_ptr<Transform> _followPtr, EmitterType _type, VEC2 _offset, float _startDelay, float _lifetime, float _particleLifetime, float _fadeSpeed,
								float _emitInterval, float _emitNum, VEC2 _emitArea, float _angle, float _movementSpeed, std::string _archetype)
	: Emitter{ _type, _startDelay, _lifetime, _particleLifetime, _fadeSpeed, _emitInterval, _emitNum, _emitArea, _archetype }, followPtr{ _followPtr }, offset{_offset}
{
	UNREFERENCED_PARAMETER(_angle);
	UNREFERENCED_PARAMETER(_movementSpeed);
}

EmitterDynamic::~EmitterDynamic()
{
}

void EmitterDynamic::CreateParticles()
{
	std::cout << "emitter : " << archeTypeName << std::endl;
	std::shared_ptr<GOC> objPtr = LOGIC->CreateObjectAt(followPtr->GetPosition() + offset, 0.0f, archeTypeName, archeTypeName);

	particleList.push_back(std::make_pair(particleLifetime, objPtr->has(SpriteComponent)));
}

void EmitterDynamic::Update(float _dt)
{
	UNREFERENCED_PARAMETER(_dt);
	startDelay -= _dt;

	if (startDelay > 0.0f)
		return;

	if (lifetime != EMITTER_INFINITE_LIFETIME)
		lifetime -= _dt;

	if (lifetime <= 0.0f)
		isActive = false;

	if (isActive)
	{
		if (intervalCounter != EMITTER_INFINITE_LIFETIME)
			intervalCounter -= _dt;

		if (intervalCounter <= 0.0f)
		{
			intervalCounter = emitInterval;
			CreateParticles();
		}
	}

	for (auto itr = std::begin(particleList); itr != std::end(particleList); )
	{
		// update position
		itr->second->GetBase()->has(Transform)->SetPosition(followPtr->GetPosition() + offset);

		if (particleLifetime != EMITTER_INFINITE_LIFETIME)
			itr->first -= _dt;

		if (itr->first <= 0.0f)
		{
			itr->second->GetBase()->Destroy();
			particleList.erase(itr++);
		}
		else
		{
			++itr;
		}
	}
}
