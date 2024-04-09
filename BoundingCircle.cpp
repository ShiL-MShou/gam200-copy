#include "stdafx.h"
#include "BoundingCircle.h"

BoundingCircle::BoundingCircle(VEC2 _coords, VEC2 _vel, float _radius):
	m_centerCoords{_coords},
	m_velocity{_vel},
	m_radius{_radius}
{
}

void BoundingCircle::SetCenterCoords(VEC2 _coords)
{
	m_centerCoords = _coords;
}

VEC2 BoundingCircle::GetCenterCoords()
{
	return m_centerCoords;
}

void BoundingCircle::SetVelocity(VEC2 _vel)
{
	m_velocity = _vel;
}

VEC2 BoundingCircle::GetVelocity()
{
	return m_velocity;
}

void BoundingCircle::SetRadius(float _radius)
{
	m_radius = _radius;
}

float BoundingCircle::GetRadius()
{
	return m_radius;
}

BoundingCircle::~BoundingCircle()
{}