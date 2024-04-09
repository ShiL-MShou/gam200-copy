/*****************************************************************************/
/*
File Name:		TextSerialization.h
Project Name:	WhereGotLate
Author(s):		Ong Cheng Kai Marcus [50%]
				Tay Qin Wen Alvin	 [50%]

Copyright information:
All content copyright 2020 DigiPen (SINGAPORE) Corporation, all rights reserved.
*/
/*****************************************************************************/
#pragma once
#include "Component.h"
#include "Utility.h"
#include "Vector2.h"
#include "Composition.h"

#include "Transform.h"

enum class CollideTypes
{
	PLAYER = 0,
	COLLECTABLE,
	INTERACTABLE,
	NPC,
	ENVIRONMENT,
	DOOR,
	WINCOLLIDER,
	BUTTON,
	NONE,
	NUMCOLLIDETYPES
};

struct AABB
{
	VEC2 minPt;
	VEC2 maxPt;

	AABB()
	{
	}

	AABB(VEC2 _min, VEC2 _max)
		: minPt{ _min }, maxPt{ _max }
	{
	}

	bool CheckCollided(AABB const* _rhs);

};

class BoxCollider : public Component
{
	friend class Physics;
	//This class just holds the info to do collision, which is done in
	//the physics system.

	CollideTypes myType;

	VEC2 m_halfLengths; //such that m_halfLengths.x measures x length from center to edge, etc
	VEC2 m_velocity;	//if no velocity, default will be 0 and 0
	bool m_canCollide;	//this determines if the aabb is "phsyical" or is just a trigger
	//bool mInteractable; // determine if the object is an interactable

	VEC2 m_offSet;

	std::shared_ptr<Transform> m_transform;

	AABB collidedAABB;
	std::weak_ptr<GOC> collidedWith;
	bool isCollided;

	std::vector<std::tuple<GOCId, AABB>> collidedlist;


public:
	VEC2 m_centerCoords;//the center of the collision box

	//unsigned int mID;

	//various constructors
	BoxCollider();// {};
	BoxCollider(float _x, float _y, float _halfWidth, float _halfheight, float _vecX = 0.0f, float _vecY = 0.0f, bool _canCollide = true);
	BoxCollider(VEC2 _centerCoords, VEC2 _halflengths, VEC2 _velocity = VEC2(), bool _canCollide = true);
	BoxCollider(BoxCollider& _bc);

	void Initialize() override;

	void Serialize(JsonReader& _jr) override;

	void DeSerialize(JsonWriter& _jw) override;

	void Update(float _dt);

	std::shared_ptr<Component> Clone() override;
	void Inspect() override;

	void Serialize(ISerialization& _stream);

	void Destroy() override;

	//Setters and Getters:
	void SetCenterCoords(VEC2 _v);
	VEC2 GetCenterCoords() const;

	void SetHalfLengths(VEC2 _v);
	VEC2 GetHalfLengths() const;
	VEC2 GetLengths() const { return m_halfLengths; }

	void SetVelocity(VEC2 _v);
	VEC2 GetVelocity();

	void SetOffSets(VEC2 _v);
	VEC2 GetOffSets();

	void SetCanCollide(bool _b);
	bool GetCanCollide() const;

	bool IsCollided();
	void ResetIsCollide();

	AABB GetCollidedAABB();

	CollideTypes GetCollidedType();

	void ClearCollideList();

	std::shared_ptr<GOC> GetCollidedObj();

	std::vector<std::tuple<GOCId, AABB>> GetCollidedList();

	void SetCollided(GOCId _id, AABB _aabb);

	CollideTypes GetMyType();


	~BoxCollider();
};