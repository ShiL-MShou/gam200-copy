/************************************************************************************ /
/*
File Name:		EmitterStatic.h
Project Name:	WhereGotLate
Author(s):		Tay Qin Wen Alvin [100%]


Brief:			Static emitter.


Copyright Information:
All content © 2019 DigiPen (SINGAPORE) Corporation, all rights reserved.
*/
/*************************************************************************************/


#pragma once
#include "Emitter.h"


class EmitterStatic :	public Emitter
{
	VEC2 position;
	float angle;
	float movementSpeed;
	

public:
	EmitterStatic(EmitterType _type, VEC2 _position, float _startDelay, float _lifetime, float _particleLifetime, float _fadeSpeed, float _emitInterval, float _emitNum, VEC2 _emitArea, float _angle, float _movementSpeed, std::string _archetype);
	~EmitterStatic();

	void CreateParticles() override;

	void Update(float _dt) override;

	void Destroy() override;
};

