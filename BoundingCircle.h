#pragma once
#include "Vector2.h"
#include "Component.h"

class BoundingCircle : public Component
{
	VEC2 m_centerCoords;
	VEC2 m_velocity;
	float m_radius;

public:
	BoundingCircle(VEC2 _coords, VEC2 _vel, float _radius);

	/*
	Gettors and Settors
	*/
	void SetCenterCoords(VEC2 _coords);
	VEC2 GetCenterCoords();

	void SetVelocity(VEC2 _vel);
	VEC2 GetVelocity();

	void  SetRadius(float _radius);
	float GetRadius();

	~BoundingCircle();
};

