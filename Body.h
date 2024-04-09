/*****************************************************************************/
/*
File Name:		Body.h
Project Name:	WhereGotLate
Author(s):		Ong Cheng Kai Marcus [40%]
				Kok Zhe Yan			 [60%]

Copyright information:
All content copyright 2020 DigiPen (SINGAPORE) Corporation, all rights reserved.
*/
/*****************************************************************************/
#pragma once

#include "Composition.h"
#include "Vector2.h"
#include "Transform.h"
#include "Force.h"
//#include "BoxCollider.h"


class BoxCollider;
class ForceManager;

class Body : public Component
{
	friend class ForceManager;
	// tmp
	VEC2 m_size;

	// KIV for physics
	VEC2 m_position;
	VEC2 m_prevPosition;
	VEC2 m_velocity;// (10, 10); //get the normalized unit vector of the direction of the velocity
	VEC2 m_acceleration;
	float m_mass;
	float m_inverseMass; //basically 1 divided by mass
	VEC2 m_sumOfForces;
	float m_maxSpeed;

	std::shared_ptr<Transform>		transform;

	
	//std::vector<std::shared_ptr<Force>> BodyForceVector;

	std::map<std::string, std::shared_ptr<Force>> forcemap;

	friend class Physics;


public:

	//unsigned int mID;

	Body();
	~Body();

	// to draw debug information like collision.
	void DebugDraw();


	virtual void Initialize() override;
	virtual std::shared_ptr<Component> Clone() override;
	virtual void Inspect() override;
	virtual void Serialize(ISerialization& _stream) override;
	virtual void Serialize(JsonReader& _jr) override;
	virtual void DeSerialize(JsonWriter& _jw) override;
	virtual void Destroy() override;

	// set and get
	void SetPosition(VEC2 _pos);
	VEC2 GetPosition() const;

	VEC2 GetVelocity();
	void SetVelocity(const VEC2& _vec);

	void SetVelocityX(float _x);
	void SetVelocityY(float _y);

	float GetMaxSpeed();
	void SetMaxSpeed(float _speed);

	std::shared_ptr<Transform> GetTransform() const;

	void SetPrevPos(VEC2 _prevPos) { m_prevPosition = _prevPos; }
	VEC2 GetPos() const { return m_position; }

	void SetPosX(float _x);
	void SetPosY(float _y);

	VEC2 GetPrevPos() const { return m_prevPosition; }

	//void AddForce(std::shared_ptr<Force> _force);
	void ActivateForces(std::string _key);
	void DeactivateForces(std::string _key);
	//void ActivateForces(ForceVecMember _key);
	void ForceLifetimeUpdate(float _dt);
	void ApplyActiveForces();
	void Integrate(float _dt);
};

