/*****************************************************************************/
/*
File Name:		Physics.h
Project Name:	WhereGotLate
Author(s):		Ong Cheng Kai Marcus [80%]
				Kok Zhe Yan			 [20%]

Copyright information:
All content copyright 2020 DigiPen (SINGAPORE) Corporation, all rights reserved.
*/
/*****************************************************************************/
#pragma once
#include "System.h"
#include "BoxCollider.h"
#include "Engine.h"
#include "Body.h"
#include "Force.h"

class ForceManager;

class Physics : public ISystem
{
	std::set<CompId> BodyList;

	std::array<
			std::set<CompId>,
			static_cast<std::underlying_type_t<CollideTypes>>(CollideTypes::NUMCOLLIDETYPES)
	> MainBCArray;


	friend class ForceManager;


public:
	std::vector<std::shared_ptr<Force>>	ForceVector;

	void Initialize() override;
	void Update(float _dt) override;
	void Clear() override;
	void SendMessage(Message& _message);
	void AABBCollisionResponse(Body& _bc1, const Body& _bc2);
	bool AABBIsColliding(std::shared_ptr<BoxCollider> _lhs, std::shared_ptr<BoxCollider> _rhs, float _dt);
	bool PointToAABBIsColliding(const VEC2& _point, const BoxCollider& _aabb);
	bool LineToAABBisColliding(VEC2 _line1, VEC2 _line2, AABB _aabb);
	bool LineToLineisColliding(VEC2 a1, VEC2 a2, VEC2 b1, VEC2 b2);


	bool PointToAABBIsColliding(const VEC2& _point, const AABB& _aabb);

	void AddBoxColliderToArray(CompId _id, CollideTypes _type);
	void AddBody(CompId _id);

	void RemoveBoxCollider(CompId _id, CollideTypes _type);
	void RemoveBody(CompId _id);

	bool IsCollidingWithMouse(std::shared_ptr<BoxCollider> _boxCollider);

	bool IsCollidingWithMouse(VEC2 _centerpos, VEC2 _halflengths);

	std::weak_ptr<GOC> GetPlayerGOC();

	Physics();
	~Physics();

	std::string GetName() override { return "Physics"; }

	std::set<CompId>& GetBoxCollidersList(CollideTypes _type);
};

extern std::shared_ptr<Physics> PHYSICS;
