/************************************************************************************ /
/*
File Name:		EmitterStatic.cpp
Project Name:	WhereGotLate
Author(s):		Tay Qin Wen Alvin [100%]


Brief:			Static emitter.


Copyright Information:
All content © 2019 DigiPen (SINGAPORE) Corporation, all rights reserved.
*/
/*************************************************************************************/

#include "stdafx.h"
#include "EmitterStatic.h"

#include "Logic.h"
#include "Graphics.h"


EmitterStatic::EmitterStatic(EmitterType _type, VEC2 _position, float _startDelay, float _lifetime, float _particleLifetime, float _fadeSpeed, float _emitInterval, float _emitNum, VEC2 _emitArea, float _angle, float _movementSpeed, std::string _archetype)
	: Emitter{ _type, _startDelay, _lifetime, _particleLifetime, _fadeSpeed, _emitInterval, _emitNum, _emitArea, _archetype }, position{ _position }, angle{ _angle }, movementSpeed{ _movementSpeed }
{
}

EmitterStatic::~EmitterStatic()
{
}

void EmitterStatic::CreateParticles()
{	
	VEC2 tmpPos = VEC2(	position.x + static_cast<float>(rand()) / static_cast<float>(RAND_MAX / emitArea.x), 
						position.y + static_cast<float>(rand()) / static_cast<float>(RAND_MAX / emitArea.y) );


	std::shared_ptr<GOC> objPtr = LOGIC->CreateObjectAt(tmpPos, 0.0f, archeTypeName, archeTypeName);

	particleList.push_back(std::make_pair(particleLifetime, objPtr->has(SpriteComponent)));

	//Using Map
	objPtr->has(Body)->ActivateForces("Gravity");

	//Using Vector
	//objPtr->has(Body)->ActivateForces(GRAVITY);

	// changing the animation
	if (type == EmitterType::STATIC_RANDOM)
	{
		objPtr->has(SpriteComponent)->ChangeAnimation(static_cast<int>(static_cast<float>(rand()) / static_cast<float>(RAND_MAX / (15.0f))));
		
		objPtr->has(Transform)->SetRotation(static_cast<float>(rand()) / static_cast<float>(RAND_MAX / (360.0f)));
	}

}

void EmitterStatic::Update(float _dt)
{
	startDelay -= _dt;

	if (startDelay > 0.0f)
		return;

	lifetime -= _dt;

	if (lifetime <= 0.0f)
		isActive = false;

	if (isActive)
	{
		intervalCounter -= _dt;

		if (intervalCounter <= 0.0f)
		{
			intervalCounter = emitInterval;
			CreateParticles();
		}
	}

	for (auto itr = std::begin(particleList); itr != std::end(particleList); )
	{
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

void EmitterStatic::Destroy()
{
}
