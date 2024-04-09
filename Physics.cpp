/*****************************************************************************/
/*
File Name:		Physics.cpp
Project Name:	WhereGotLate
Author(s):		Ong Cheng Kai Marcus [80%]
				Kok Zhe Yan			 [20%]

Copyright information:
All content copyright 2020 DigiPen (SINGAPORE) Corporation, all rights reserved.
*/
/*****************************************************************************/
#include "stdafx.h"
#include "Physics.h"
#include "ComponentCreator.h"
#include "Body.h"
#include "Logic.h"

#include "DebugDraw.h"
#include "CoreEngine.h"
#include "ForceManager.h"
#include "MyGuiManager.h"

#include "Input.h" // for mouse position
#include "TimeManager.h"


std::shared_ptr<Physics> PHYSICS;

Physics::Physics() : MainBCArray{}
{
	
}

Physics::~Physics()
{
	//MyForceManager.Clear();
}

std::set<CompId>& Physics::GetBoxCollidersList(CollideTypes _type)
{
	return MainBCArray[static_cast<std::underlying_type_t<CollideTypes>>(_type)];
}

void Physics::Initialize()
{
	RegisterComponent(Body);
	RegisterComponent(BoxCollider);
	MyForceManager.Initialize();
}



void Physics::Update(float _dt)
{
	//for (auto iter : BoxColliderVector)

	//std::cout << CORE->GetCurrLevel() << std::endl;

	std::shared_ptr<SpriteComponent> spritePtr;


	// drawing force velocity line
	for (auto id : BodyList)
	{
		MyResource.GetComponentWithID<Body>(id)->DebugDraw();
	}

	if (CORE->GetPausedStatus() || CORE->GetEditorStatus() && MyGuiManager::isGamePaused)
	{
		for (int i = 0; i < MainBCArray.size(); ++i)
			for (auto id : MainBCArray[i])
			{
				std::shared_ptr<BoxCollider> bc = MyResource.GetComponentWithID<BoxCollider>(id);
				std::shared_ptr<Transform> m_transform2 = bc->GetBase()->has(Transform);

				spritePtr = bc->GetBase()->has(SpriteComponent);


				bc->m_centerCoords = m_transform2->GetPosition() + bc->m_offSet;

				MyDebugDraw.DrawRect(bc->m_centerCoords, bc->m_halfLengths);
			}
		return;
	}

	//for (int step = 0; step < CORE->currNumSteps; ++step)
	{
		//get all the colliders
		for (auto id : BodyList)
		{
			//ptr->GetTransform()->SetPosition(ptr->GetTransform()->GetPosition() + ptr->GetVelocity() * _dt);

			std::shared_ptr<Body> body = MyResource.GetComponentWithID<Body>(id);

			//std::cout << "Phy checking: " << body->GetBase()->GetName() << std::endl;
		/*	if (strcmp(body->GetBase()->GetName(), "Player") == 0)
				body->GetVelocity().printVEC2();*/
			body->SetPrevPos(body->GetPosition());

			body->ForceLifetimeUpdate(_dt);
			/*if (strcmp(body->GetBase()->GetName(), "Player") == 0)
				std::cout << "Applying \n" << std::endl;*/
			body->ApplyActiveForces();
			/*if (strcmp(body->GetBase()->GetName(), "Player") == 0)
				std::cout << "\nApplied....... " << std::endl;*/
			body->Integrate(_dt);

		}

	}

	for (int i = 0; i < MainBCArray.size(); ++i)
		for (auto id : MainBCArray[i])
		{
			std::shared_ptr<BoxCollider> bc = MyResource.GetComponentWithID<BoxCollider>(id);
			std::shared_ptr<Transform> m_transform2 = bc->GetBase()->has(Transform);

			spritePtr = bc->GetBase()->has(SpriteComponent);


			bc->m_centerCoords = m_transform2->GetPosition() + bc->m_offSet;

			MyDebugDraw.DrawRect(bc->m_centerCoords, bc->m_halfLengths);
		}

	std::shared_ptr<BoxCollider> player = !MainBCArray[0].empty() ? MyResource.GetComponentWithID<BoxCollider>(*MainBCArray[0].begin()) : nullptr;

	for (int i = 1; i < 7; ++i) //size should be 7 , Array[6] is still something. 
	{
		//if (MainBCArray[0].empty())
			//break;

		for (auto id : MainBCArray[i])
		{
			/*if (MainBCArray[i].empty())
				continue;*/

			std::shared_ptr<BoxCollider> bc = MyResource.GetComponentWithID<BoxCollider>(id);
			//bc->ClearCollideList();

			if (MainBCArray[0].size() == 0)
				break;

			
			

			//if (bc == player)
				//continue;

			// we manually choose the Player's BC to check against MainBCArray[i][j]
			//check to see if the colliders are colliding with player
			if ((player->GetCanCollide() == true) &&
				(bc->GetCanCollide() == true))
			{

				if (AABBIsColliding(player, bc, _dt))
				{
					//AABBCollisionResponse( *(BodyVector[i]), *(BodyVector[j]) );
					/*if (i == 4)
					{
						std::cout << player->GetCenterCoords().x + player->GetHalfLengths().x << " | " << bc->GetCenterCoords().x - player->GetHalfLengths().x << std::endl;
						std::cout << player->GetCenterCoords().x - player->GetHalfLengths().x << " | " << bc->GetCenterCoords().x + player->GetHalfLengths().x << std::endl;
						std::cout << player->GetCenterCoords().y + player->GetHalfLengths().y << " | " << bc->GetCenterCoords().y - player->GetHalfLengths().y << std::endl;
						std::cout << player->GetCenterCoords().y - player->GetHalfLengths().y << " | " << bc->GetCenterCoords().y + player->GetHalfLengths().y << std::endl;
					}*/

					player->SetCollided(bc->GetBase()->GetId(),
						AABB(VEC2(bc->GetCenterCoords() - bc->GetHalfLengths()),
							VEC2(bc->GetCenterCoords() + bc->GetHalfLengths()))
					);

					bc->SetCollided(player->GetBase()->GetId(),
						AABB(VEC2(player->GetCenterCoords() - player->GetHalfLengths()),
							VEC2(player->GetCenterCoords() + player->GetHalfLengths()))
					);

					//std::cout << BodyVector[i]->mID << " colliding with " << BodyVector[j]->mID << std::endl;
				}
			}

			for (auto NPCID : MainBCArray[3]) //iterator for NPC portion of MainBCArray
			{
				std::shared_ptr<BoxCollider> NPC = MyResource.GetComponentWithID<BoxCollider>(NPCID);
				//NPC->ClearCollideList();
				
				if (!(i == 1 || i == 5))//bc == NPC)
					continue;
				//for (auto IterCollectableID : MainBCArray[1])//iterator for Collectables
				//{
					//std::shared_ptr<BoxCollider> IterCollectable = MyResource.GetComponentWithID<BoxCollider>(IterCollectableID);

					if (NPC->GetCanCollide() == true &&
						bc->GetCanCollide() == true)
					{
						if (AABBIsColliding(NPC, bc, _dt)) //check if NPCs collide with Collectables
						{
							//std::cout << bc->GetBase()->GetName()  << " & " << NPC->GetBase()->GetName() << std::endl;
							NPC->SetCollided(bc->GetBase()->GetId(),
								AABB(VEC2(bc->GetCenterCoords() - bc->GetHalfLengths()),
									VEC2(bc->GetCenterCoords() + bc->GetHalfLengths()))
							);

							bc->SetCollided(NPC->GetBase()->GetId(),
								AABB(VEC2(NPC->GetCenterCoords() - NPC->GetHalfLengths()),
									VEC2(NPC->GetCenterCoords() + NPC->GetHalfLengths()))
							);
						}
					}
				//}
			}
		}
	}
}

void Physics::Clear()
{
	BodyList.clear();
	//BoxColliderVector.clear();
	//InteractableColliderVector.clear();
	ForceVector.clear();
	//MyForceManager.Clear();
	for (int i = 0; i < MainBCArray.size(); ++i)
		MainBCArray[i].clear();		
}

void Physics::SendMessage(Message& _message)
{
	UNREFERENCED_PARAMETER(_message);

	switch (_message.m_Mid)
	{
	case MessageId::None:
		break;
	}

}

/*
	Handles collision response. WIP
*/
void Physics::AABBCollisionResponse(Body& _bc1, const Body& _bc2)
{
	float testx = _bc1.GetTransform()->GetPosition().x;
	float testy = _bc1.GetTransform()->GetPosition().y;

	if ((_bc1.GetTransform()->GetPosition().x - _bc2.GetTransform()->GetPosition().x) < 0.0f)
	{
		testx -= 1.0f;
	}
	else if (_bc1.GetTransform()->GetPosition().x > _bc2.GetTransform()->GetPosition().x)
	{
		testx += 1.0f;
	}
	
	if (_bc1.GetTransform()->GetPosition().y > _bc2.GetTransform()->GetPosition().y)
	{
		testy += 1.0f;
	}
	else if (_bc1.GetTransform()->GetPosition().y < _bc2.GetTransform()->GetPosition().y)
	{
		testy -= 1.0f;
	}
	
	_bc1.GetTransform()->SetPosition({testx, testy});
	_bc1.SetVelocity({0.0f, 0.0f});
}

bool Physics::PointToAABBIsColliding(const VEC2& _point, const BoxCollider& _aabb)
{
	VEC2 aabbmin{ _aabb.GetCenterCoords() - _aabb.GetHalfLengths() };
	VEC2 aabbmax{ _aabb.GetCenterCoords() + _aabb.GetHalfLengths() };

	if (_point.x < aabbmin.x || _point.x > aabbmax.x||
		_point.y < aabbmin.y || _point.y > aabbmax.y )
	{
		return false;
	}
	return true;
}

bool Physics::LineToAABBisColliding(VEC2 _line1, VEC2 _line2, AABB _aabb)
{
	VEC2 aabbmin = _aabb.minPt;
	VEC2 aabbmax = _aabb.maxPt;

	if (LineToLineisColliding(aabbmin, VEC2{ aabbmin.x, aabbmax.y }, _line1, _line2))
	{
		return true;
	}
	if (LineToLineisColliding(aabbmin, VEC2{ aabbmax.x, aabbmin.y }, _line1, _line2))
	{
		return true;
	}
	if (LineToLineisColliding(VEC2{ aabbmax.x, aabbmin.y }, aabbmax, _line1, _line2))
	{
		return true;
	}
	if (LineToLineisColliding(VEC2{ aabbmin.x, aabbmax.y }, aabbmax, _line1, _line2))
	{
		return true;
	}

	return false;
}

bool Physics::LineToLineisColliding(VEC2 a1, VEC2 a2, VEC2 b1, VEC2 b2)
{
	//intersection = {0,0};

	VEC2 b = a2 - a1;
	VEC2 d = b2 - b1;
	float bDotDPerp = b.x * d.y - b.y * d.x;

	// if b dot d == 0, it means the lines are parallel so have infinite intersection points
	if (bDotDPerp == 0)
		return false;

	VEC2 c = b1 - a1;
	float t = (c.x * d.y - c.y * d.x) / bDotDPerp;
	if (t < 0 || t > 1)
		return false;

	float u = (c.x * b.y - c.y * b.x) / bDotDPerp;
	if (u < 0 || u > 1)
		return false;

	//intersection = a1 + t * b;

	return true;
}

bool Physics::PointToAABBIsColliding(const VEC2& _point, const AABB& _aabb)
{
	VEC2 aabbmin = _aabb.minPt;
	VEC2 aabbmax = _aabb.maxPt;

	if (_point.x < aabbmin.x || _point.x > aabbmax.x ||
		_point.y < aabbmin.y || _point.y > aabbmax.y)
	{
		return false;
	}
	return true;
}


void Physics::AddBoxColliderToArray(CompId _id, CollideTypes _type)
{
	//if (_bc->GetCollidedType() == CollideTypes::PLAYER)

	int it = static_cast<std::underlying_type_t<CollideTypes>>(_type);
	MainBCArray[it].insert(_id);
}


void Physics::AddBody(CompId _id)
{
	BodyList.insert(_id);
}

void Physics::RemoveBoxCollider(CompId _id, CollideTypes _type)
{
	int it = static_cast<std::underlying_type_t<CollideTypes>>(_type);
	MainBCArray[it].erase(_id);
}

void Physics::RemoveBody(CompId _id)
{
	BodyList.erase(_id);

}
bool Physics::IsCollidingWithMouse(std::shared_ptr<BoxCollider> _boxCollider)
{
	VEC2 mousePos = GRAPHICS->ScreenToWorld(INPUTMGR->GetMousePosition());

	AABB aabb1(_boxCollider->GetCenterCoords() - _boxCollider->GetHalfLengths(), _boxCollider->GetCenterCoords() + _boxCollider->GetHalfLengths());

	if (mousePos.x > aabb1.minPt.x && mousePos.x < aabb1.maxPt.x &&
		mousePos.y > aabb1.minPt.y && mousePos.y < aabb1.maxPt.y)
	{
		return true;
	}

	return false;
}
bool Physics::IsCollidingWithMouse(VEC2 _centerpos, VEC2 _halflengths)
{
	VEC2 mousePos = GRAPHICS->ScreenToWorld(INPUTMGR->GetMousePosition());

	AABB aabb1(_centerpos - _halflengths, _centerpos + _halflengths);

	if (mousePos.x > aabb1.minPt.x&& mousePos.x < aabb1.maxPt.x &&
		mousePos.y > aabb1.minPt.y&& mousePos.y < aabb1.maxPt.y)
	{
		return true;
	}

	return false;
}

std::weak_ptr<GOC> Physics::GetPlayerGOC()
{
	std::shared_ptr<BoxCollider> player = MyResource.GetComponentWithID<BoxCollider>(*MainBCArray[0].begin());

	return player->GetBase();

	//return MyResource.GetObjectWithID(*MainBCArray[0].begin());
}
/*
	implementation of basic AABB collision
*/
bool Physics::AABBIsColliding(std::shared_ptr<BoxCollider> _lhs, std::shared_ptr<BoxCollider> _rhs, float _dt)
{
	UNREFERENCED_PARAMETER(_dt);
	AABB aabb1{ _lhs->GetCenterCoords() - _lhs->GetHalfLengths(), _lhs->GetCenterCoords() + _lhs->GetHalfLengths() };
	AABB aabb2{ _rhs->GetCenterCoords() - _rhs->GetHalfLengths(), _rhs->GetCenterCoords() + _rhs->GetHalfLengths() };
	//std::cout << _lhs->GetCenterCoords().x << ", " << _lhs->GetCenterCoords().y << " | " << _rhs->GetCenterCoords().x << ", " << _rhs->GetCenterCoords().y << std::endl;
	//VEC2 aabb1min{ _lhs->GetCenterCoords() - _lhs->GetHalfLengths() };
	//VEC2 aabb1max{ _lhs->GetCenterCoords() + _lhs->GetHalfLengths() };

	//VEC2 aabb2min{ _rhs->GetCenterCoords() - _rhs->GetHalfLengths() };
	//VEC2 aabb2max{ _rhs->GetCenterCoords() + _rhs->GetHalfLengths() };

	if (!(aabb1.maxPt.x < aabb2.minPt.x || aabb1.maxPt.y < aabb2.minPt.y ||
		aabb1.minPt.x > aabb2.maxPt.x || aabb1.minPt.y > aabb2.maxPt.y))
	{
		return true;
	}
	else
		return false;
	//std::shared_ptr<Body> lhs = _lhs->GetBase()->has(Body);
	//std::shared_ptr<Body> rhs = _rhs->GetBase()->has(Body);
	////std::cout << lhs->GetVelocity().x << ", " << lhs->GetVelocity().y << std::endl;
	////std::cout << _lhs->GetBase()->GetName() << " | " << _rhs->GetBase()->GetName() << std::endl;
	//VEC2 avel = lhs ? lhs->GetVelocity() : VEC2{ 0.0f, 0.0f };
	//VEC2 bvel = rhs ? rhs->GetVelocity() : VEC2{ 0.0f, 0.0f };

	//VEC2 vrel(bvel - avel);// .x - _lhs->GetVelocity().x, _rhs->GetVelocity().y - _lhs->GetVelocity().y);
	//
	//if (vrel.x == 0.0f && vrel.y == 0.0f)
	//	return false;
	//else
	//{
	//	//std::cout << vrel.x << ", " << vrel.y << std::endl;
	//}

	//float tfirst = 0.0f;
	//float tlast = MyTimeManager.GetDt();
	//float tmp = 0;

	//// check x axis for collision
	//if (vrel.x < 0)
	//{
	//	if (aabb1.maxPt.x < aabb2.minPt.x)
	//	{
	//		tmp = (aabb1.maxPt.x - aabb2.minPt.x) / vrel.x;
	//		tfirst = std::max(tmp, tfirst);
	//	}
	//	if (aabb1.minPt.x < aabb2.maxPt.x)
	//	{
	//		tmp = (aabb1.minPt.x - aabb2.maxPt.x) / vrel.x;
	//		tlast = std::min(tmp, tlast);
	//	}
	//	else
	//		return false;
	//}
	//else if (vrel.x > 0)
	//{
	//	if (aabb1.minPt.x > aabb2.maxPt.x)
	//	{
	//		tmp = (aabb1.minPt.x - aabb2.maxPt.x) / vrel.x;
	//		tfirst = std::max(tmp, tfirst);
	//	}
	//	if (aabb1.maxPt.x > aabb2.minPt.x)
	//	{
	//		tmp = (aabb1.maxPt.x - aabb2.minPt.x) / vrel.x;
	//		tlast = std::min(tmp, tlast);
	//	}
	//	else
	//		return false;
	//}
	///*else
	//{
	//	tfirst = std::max(tfirst, _dt);
	//	tlast = std::min(tlast, 0.0f);
	//}*/
	//if (tfirst > tlast)
	//	return false;

	//// check y axis for collision
	//if (vrel.y < 0)
	//{
	//	if (aabb1.maxPt.y < aabb2.minPt.y)
	//	{
	//		tmp = (aabb1.maxPt.y - aabb2.minPt.y) / vrel.y;
	//		tfirst = std::max(tmp, tfirst);
	//	}
	//	if (aabb1.minPt.y < aabb2.maxPt.y)
	//	{
	//		tmp = (aabb1.minPt.y - aabb2.maxPt.y) / vrel.y;
	//		tlast = std::min(tmp, tlast);
	//	}
	//	else
	//		return false;
	//}
	//else if (vrel.y > 0)
	//{
	//	if (aabb1.minPt.y > aabb2.maxPt.y)
	//	{
	//		tmp = (aabb1.minPt.y - aabb2.maxPt.y) / vrel.y;
	//		tfirst = std::max(tmp, tfirst);
	//	}
	//	if (aabb1.maxPt.y > aabb2.minPt.y)
	//	{
	//		tmp = (aabb1.maxPt.y - aabb2.minPt.y) / vrel.y;
	//		tlast = std::min(tmp, tlast);
	//	}
	//	else
	//		return false;
	//}
	///*else
	//{
	//	tfirst = std::max(tfirst, _dt);
	//	tlast = std::min(tlast, 0.0f);
	//}*/

	//if (tfirst > tlast)
	//	return false;

	////std::cout << _rhs->GetBase()->GetName() << std::endl;
	//return true;

	//if (relativeVel.x == 0.0f && relativeVel.y == 0.0f)
	//	return false;

	//double tFirst = 0;
	//double tLast  = _dt;

	//if (relativeVel.x < 0) //if relative x-axis velocity of ab2 is to left
	//{
	//	if (aabb1min.x > aabb2max.x) //case 1, no collision, aabb1 is to the right of aabb2
	//	{
	//		return false;
	//	}
	//	else if (aabb1max.x < aabb2min.x) //case 4, tFirst. aabb1 on left of aabb2, aabb2 moving to aabb1
	//	{
	//		if ((aabb1max.x - aabb2max.x) / relativeVel.x > tFirst)
	//		{
	//			tFirst = (aabb1max.x - aabb2max.x) / relativeVel.x;
	//		}
	//	}
	//	else if (aabb1min.x)//case 4 tLast
	//	{
	//		if ((aabb1min.x - aabb2max.x) / relativeVel.x < tLast)
	//			tLast = (aabb1min.x - aabb2max.x) / relativeVel.x;
	//	}
	//}
	//else if (relativeVel.x > 0)// if ab2 vel is to the right
	//{
	//	if (aabb1max.x < aabb2min.x)//case 3, no collision. aabb2 is to the right of aabb1
	//	{
	//		return 0;
	//	}
	//	else if (aabb1min.x > aabb2max.x)//case 2, tFirst. aabb2 is to the right of aabb1 (moving to aabb1)
	//	{
	//		if ((aabb1min.x - aabb2max.x) / relativeVel.x > tFirst)
	//			tFirst = (aabb1min.x - aabb2max.x) / relativeVel.x;
	//	}
	//	else if (aabb1max.x > aabb2min.x)//case 2, tLast
	//	{
	//		if ((aabb1max.x - aabb2min.x) / relativeVel.x < tLast)
	//			tLast = (aabb1max.x - aabb2min.x) / relativeVel.x;
	//	}
	//}
	//if (tFirst > tLast)
	//{
	//	return false;
	//}

	//if (relativeVel.y < 0)//if relative velocity of aabb2(y coord) is towards the bottom
	//{
	//	if (aabb1min.y > aabb2max.y)//case 1, no collision. aabb2 is below aabb1
	//	{
	//		return false;
	//	}
	//	else if (aabb1max.y < aabb2min.y)//case 4, tFirst. aabb1 is below aabb2 (aabb2 moving to aabb1)
	//	{
	//		if ((aabb1max.y - aabb2min.y) / relativeVel.y > tFirst)
	//			tFirst = (aabb1max.y - aabb2min.y) / relativeVel.y;
	//	}
	//	else if (aabb1min.y < aabb2max.y)//case 4, tLast
	//	{
	//		if ((aabb1min.y - aabb2max.y) / relativeVel.y < tLast)
	//			tLast = (aabb1min.y - aabb2max.y) / relativeVel.y;
	//	}
	//}
	//else if (relativeVel.y > 0)//if relative velocity of aabb2(y coord) is towards the top
	//{
	//	if (aabb1max.y < aabb2min.y)//case 3, no collision. aabb2 is above aabb1
	//	{
	//		return false;
	//	}
	//	else if (aabb1min.y > aabb2max.y)//case 2, tFirst. aabb1 is above aabb2 (aabb2 moving to aabb1)
	//	{
	//		if ((aabb1min.y - aabb2max.y) / relativeVel.y > tFirst)
	//			tFirst = (aabb1min.y - aabb2max.y) / relativeVel.y;
	//	}
	//	else if (aabb1max.y > aabb2min.y)//case 2, tLast
	//	{
	//		if ((aabb1max.y - aabb2min.y) / relativeVel.y < tLast)
	//			tLast = (aabb1max.y - aabb2min.y) / relativeVel.y;
	//	}
	//}
	//if (tFirst > tLast)
	//	return false;

	////std::cout << _rhs->GetBase()->GetName() << std::endl;
	//return true;
}

