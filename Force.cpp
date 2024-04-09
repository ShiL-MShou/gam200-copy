/*****************************************************************************/
/*
File Name:		Force.cpp
Project Name:	WhereGotLate
Author(s):		Ong Cheng Kai Marcus [100%]

Copyright information:
All content copyright 2020 DigiPen (SINGAPORE) Corporation, all rights reserved.
*/
/*****************************************************************************/
#include "stdafx.h"
#include "Physics.h"
#include "Force.h"



Force::Force(VEC2 _dir, float _mag, float _life, FORCE_TYPE _type, bool _b, float _age) :
	m_direction{_dir},
	m_magnitude{ _mag },
	m_lifetime{ _life },
	m_isActive{_b}, //do we want force to be immediately active? nope
	m_forceType{ _type },
	m_age{ _age },
	m_epsilon{ m_magnitude }
{
	m_epsilon /= 80.0f;
}

Force::~Force()
{
}

Force::Force(const Force& rhs) :
	m_direction{ rhs.m_direction },
	m_magnitude{ rhs.m_magnitude },
	m_lifetime{ rhs.m_lifetime },
	m_forceType{ rhs.m_forceType },
	m_isActive{ rhs.m_isActive },
	m_age{ rhs.m_age },
	m_epsilon{ rhs.m_epsilon }
{

}

Force& Force::operator=(const Force& rhs)
{
	m_direction = rhs.m_direction;
	m_magnitude = rhs.m_magnitude;
	m_lifetime = rhs.m_lifetime;
	m_forceType = rhs.m_forceType;
	m_isActive = rhs.m_isActive;
	m_age = rhs.m_age;
	m_epsilon = rhs.m_epsilon;

	return *this;
}


float Force::GetAge(float _dt)
{
	UNREFERENCED_PARAMETER(_dt);
	return m_age;
}

void Force::SetLifetime(float _life)
{
	m_lifetime = _life;
}

void Force::SetActiveStatus(bool _b)
{
	m_isActive = _b;
}