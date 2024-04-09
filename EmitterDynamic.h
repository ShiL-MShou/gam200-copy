/************************************************************************************ /
/*
File Name:		EmitterDynamic.h
Project Name:	WhereGotLate
Author(s):		Tay Qin Wen Alvin [100%]


Brief:			Dynamic emitter.


Copyright Information:
All content © 2019 DigiPen (SINGAPORE) Corporation, all rights reserved.
*/
/*************************************************************************************/


#pragma once
#include "Emitter.h"

#include <memory>
#include "Transform.h"




class EmitterDynamic : public Emitter
{
	std::shared_ptr<Transform> followPtr;
	VEC2 offset;
	
	//type

public:
	EmitterDynamic();
	EmitterDynamic(	std::shared_ptr<Transform> _followPtr, EmitterType _type, VEC2 _offset, float _startDelay, float _lifetime, float _particleLifetime, float _fadeSpeed,
					float _emitInterval, float _emitNum, VEC2 _emitArea, float _angle, float _movementSpeed, std::string _archetype);

	~EmitterDynamic() override;

	EmitterDynamic(const EmitterDynamic&) = delete;
	EmitterDynamic& operator=(const EmitterDynamic&) = delete;

	void CreateParticles() override;

	void Update(float _dt) override;
};

