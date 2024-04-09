/*****************************************************************************/
/*
File Name:		Force.h
Project Name:	WhereGotLate
Author(s):		Ong Cheng Kai Marcus [100%]

Copyright information:
All content copyright 2020 DigiPen (SINGAPORE) Corporation, all rights reserved.
*/
/*****************************************************************************/
#pragma once
#include "Vector2.h"
#include "ForceTypes.h"



class Force
{
	
public:
	//enum for type, to distinguish different forces. drag etc. this will be used in body/physics for the various kinds.
	VEC2		m_direction;
	float		m_magnitude;
	float		m_lifetime;
	FORCE_TYPE	m_forceType;
	bool		m_isActive;
	float		m_age;
	float		m_epsilon;

	Force(VEC2 _dir, float _mag, float _life, FORCE_TYPE _type, bool _b = false, float _age=0.0f);
	~Force();

	Force(const Force& rhs);
	Force& operator=(const Force& rhs);

	//void Initialize();

	/********************************************************************************
	gets the current age of the force by doing 

			_dt      -        age
		(current dt) - (dt when force was created)

	so if GetAge() == m_lifetime, the force has reached the end of its lifetime, then do stuff
	*********************************************************************************/
	float GetAge(float _dt);

	void SetLifetime(float _life);

	void SetActiveStatus(bool _b);

};

