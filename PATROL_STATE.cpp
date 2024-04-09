/******************************************************************************/
/*
	File Name: PATROL_STATE.cpp
	Project Name: WhereGotLate
	Author(s): Lee Shi Lin [100%]

	Brief: Derived state for Teacher NPC

	Copyright Information:
	All content © 2019 DigiPen (SINGAPORE) Corporation, all rights reserved.
*/
/******************************************************************************/
#include "stdafx.h"
#include "PATROL_STATE.h"
#include "Transform.h"
#include "Physics.h"
#include "SpriteComponent.h"
#include "Body.h"
#include "PathFind.h"
#include "Logic.h"

#include <memory>
#include "AnimationComponent.h"
#include "Graphics.h"
#include "Animation.h"

#include "DebugDraw.h"



PATROL_STATE::PATROL_STATE()
	: patrol_pts{}, path_to_pt{}, _startPt{ 0 }, _nextPt{ 0 }, isMovingRight{ false }, _UnitPos{}, 
	_distract_des{}, speed{ 20.0f }, isFacingRight{ true }
{
	iState::_name = "Patrol";
}

PATROL_STATE::~PATROL_STATE()
{
}

void PATROL_STATE::Update(float _dt)
{
	//std::cout << "Patrol" << std::endl;


	if (IsUnableToChase() == true)
	{
		DisableAllForces(GetOwner()->has(Body));
		return;
	}
		
	
	// Walk toward the curr patrol point
	float step = speed * _dt;
	_UnitPos = GetOwner()->has(Transform)->GetPosition();
	
	//std::shared_ptr<SpriteComponent> sprPtr = GetOwner()->has(SpriteComponent);
	//std::shared_ptr<AnimationComponent> animationPtr = GetOwner()->has(AnimationComponent);
	//
	//if (sprPtr != nullptr && animationPtr != nullptr)
	//{
	//	int animationID = animationPtr->animationList.at(animationPtr->currAnimation);
	//	std::string animationName = GRAPHICS->GetAnimationName(animationID);
	//
	//	if (animationName.find("WALKING") == std::string::npos)
	//		sprPtr->ChangeGenericAnimation("WALKING");
	//}

	//_UnitPos.printVEC2();

	// when there is no more pts to move
	if (path_to_pt.empty() == true)
	{
		//std::vector<VEC2> g = ;

		SetPathtoPt(FindPath(LOGIC->GetAImap(), _UnitPos, patrol_pts[_currPt]));
		SetCurrsmallPoint();
	}

	// there is pts to move and have not reach the pt
	if (path_to_pt.size() != 0 && _UnitPos != /*Destination*/ patrol_pts[_currPt])
		MoveTo(step);

	_UnitPos = GetOwner()->has(Transform)->GetPosition();


	if (path_to_pt.size() != 0 && _UnitPos == path_to_pt[path_to_pt.size() - 1] || _UnitPos == patrol_pts[_currPt])
	{
		path_to_pt.clear();
		//SetNextPoint();
		//std::cout << "Reach location!" << std::endl;
	}

	//if (path_to_pt.size() != 0 && GetGrid_index(_UnitPos) == GetGrid_index(path_to_pt[path_to_pt.size() - 1]))
	//{
	//	path_to_pt.clear();
	//	//SetNextPoint();
	//	std::cout << "Reach location!" << std::endl;
	//}

	for (auto& tmpVec : path_to_pt)
	{
		MyDebugDraw.DrawDebug(tmpVec, tmpVec + VEC2(10, 10));
	}
}

void PATROL_STATE::OnEnter()
{
	// Get objOwner position
	_UnitPos = GetOwner()->has(Transform)->GetPosition();

	// Get path to current point to go to
	path_to_pt.clear();
	std::vector<VEC2> g = FindPath(LOGIC->GetAImap(), _UnitPos, patrol_pts[_currPt]);
	/*/
	std::cout << g.size() << std::endl;
	//for (size_t i = 0; i < g.size(); i++)
	//{
	//	g[i].printVEC2();
	//}
	std::cout << std::endl;
	/*/
	SetPathtoPt(g);
	SetCurrsmallPoint();

	
	
	if (IsUnableToChase() == true)
		return;

	// Set Animation layer
	//std::shared_ptr<SpriteComponent> sprPtr = GetOwner()->has(SpriteComponent);
	std::shared_ptr<SpriteComponent> sprPtr = GetOwner()->has(SpriteComponent);

	if (sprPtr != nullptr)
	{
	//bool isRight = GetOwner()->has(Transform)->GetScale().x > 0;
		sprPtr->ChangeGenericAnimation("WALKING");
		_ownerSM->updatedSprite = true;

		//if (!isRight)
		//	sprPtr->FlipSprite();

		/*sprPtr->SetCurrSubTextureName("WALKING");
		sprPtr->SetMaxFrame(15);
		sprPtr->SetCurrFrame(0);*/
		//GetOwner()->has(Sprite)->FlipSprite();

		//ChangeDirection(sprPtr);
	}
}

void PATROL_STATE::OnExit()
{
	//path_to_pt.clear();
	//std::cout << "change next location" << std::endl;
	SetNextPoint();
}

void PATROL_STATE::ResetState()
{
	_currPt = _startPt;
	patrol_pts.clear();
	path_to_pt.clear();
}

void PATROL_STATE::UpdateAnimation()
{
	if (IsUnableToChase() == true)
		return;
	
	GetOwner()->has(SpriteComponent)->ChangeGenericAnimation("WALKING");
	_ownerSM->updatedSprite = true;
}

void PATROL_STATE::AddPatrolPoint(VEC2 pt)
{
	VEC2 gridVec = GetGrid_index(pt);

	gridVec.x = (gridVec.x + 0.5f) * cWIDTH;
	gridVec.y = (gridVec.y + 0.5f) * cHEIGHT;

	//std::cout << "Patrol Pts: " <<  gridVec.x << " " << gridVec.y << std::endl;
	
	patrol_pts.push_back(gridVec);
}

void PATROL_STATE::SetStartPoint(int index)
{
	if (patrol_pts.size() == 0)
	{
		std::cout << "No Points of Patrol" << std::endl;
	}
	else
	{
		_startPt = index;
		_currPt = _startPt;

		if (patrol_pts.size() == 1)
			_nextPt = _currPt;
		else
		{
			if (_currPt + 1 == patrol_pts.size())
				_nextPt = 0;
			else
				_nextPt = _currPt + 1;
		}
	}
}

void PATROL_STATE::SetNextPoint()
{
	_currPt = _nextPt;

	if (patrol_pts.size() == 1)
		_nextPt = _currPt;
	else
	{
		if (_currPt + 1 == patrol_pts.size())
			_nextPt = 0;
		else
			_nextPt = _currPt + 1;
	}
}

VEC2 PATROL_STATE::GetCurrPt() //for Patrol_pts
{
	return patrol_pts[_currPt];
}

VEC2 PATROL_STATE::GetUnitPos()
{
	_UnitPos = GetOwner()->has(Transform)->GetPosition();
	return _UnitPos;
}

void PATROL_STATE::SetDistractDes(VEC2 t)
{
	_distract_des = t;
}

VEC2 PATROL_STATE::GetDistractDes()
{
	return _distract_des;
}

void PATROL_STATE::MoveTo(float step)
{
	UNREFERENCED_PARAMETER(step);

	//======================
	// OLD move to position
	//======================
	//VEC2 n = VEC2MoveTowards(_UnitPos, patrol_pts[_currPt], step);
	//GetOwner()->has(Transform)->SetPosition(n);

	//======================
	// NEW move to position
	//======================
	// Snap x


	// Snap Y
	
	//std::cout << "moving to : " << path_to_pt[_currspt].x << " |  " << path_to_pt[_currspt].y << std::endl;
	
	MoveToPos(GetOwner()->has(Body), _UnitPos, path_to_pt[_currspt]);

	/*if (VEC2Magnitude(_UnitPos, path_to_pt[_currspt]) <= 1)
		GetNextsmallPoint();*/

	//// Snap to location
	//if (VEC2Magnitude(_UnitPos, path_to_pt[_currspt]) <= 1)
	//{
	//	std::cout << "Snap" << std::endl;
	//	GetOwner()->has(Transform)->SetPosition(path_to_pt[_currspt]);

	//	GetOwner()->has(Body)->DeactivateForces("playerRight");
	//	GetOwner()->has(Body)->DeactivateForces("playerLeft");
	//	GetOwner()->has(Body)->DeactivateForces("playerUp");
	//	GetOwner()->has(Body)->DeactivateForces("playerDown");
	//	GetOwner()->has(Body)->DeactivateForces("drag");

	//	GetOwner()->has(Body)->SetVelocity(VEC2(0, 0));
	//	GetNextsmallPoint();
	//	return;
	//}

	//if (_UnitPos.x < path_to_pt[_currspt].x) // move right
	//{
	//	//std::cout << "right" << _UnitPos.y << std::endl;
	//	GetOwner()->has(Body)->ActivateForces("playerRight");
	//	GetOwner()->has(Body)->ActivateForces("drag");
	//}
	//else if (_UnitPos.x > path_to_pt[_currspt].x) // move left
	//{
	//	//std::cout << "left " << _UnitPos.y << std::endl;
	//	GetOwner()->has(Body)->ActivateForces("playerLeft");
	//	GetOwner()->has(Body)->ActivateForces("drag");
	//}

	//if (_UnitPos.y < path_to_pt[_currspt].y) // move up
	//{
	//	//std::cout << "Up " << _UnitPos.y << std::endl;
	//	GetOwner()->has(Body)->ActivateForces("playerUp");
	//	GetOwner()->has(Body)->ActivateForces("drag");
	//}
	//else if (_UnitPos.y > path_to_pt[_currspt].y) // move down
	//{
	//	//std::cout << "Down" << _UnitPos.y << std::endl;
	//	GetOwner()->has(Body)->ActivateForces("playerDown");
	//	GetOwner()->has(Body)->ActivateForces("drag");
	//}

	//std::cout << "Current Pt: " << _currPt << "  |  " << n.x << " | " << n.y << std::endl;

	//if (_UnitPos.x < n.x && GetOwner()->has(Transform)->GetScale().x < 0)
	//{
	//	GetOwner()->has(SpriteComponent)->FlipSprite();
	//}
	//else if (_UnitPos.x > n.x&& GetOwner()->has(Transform)->GetScale().x > 0)
	//{
	//	GetOwner()->has(SpriteComponent)->FlipSprite();
	//}

	GetNextsmallPoint();
}

void PATROL_STATE::SetPathtoPt(std::vector<VEC2> list)
{
	for (size_t i = 0; i < list.size(); i++)
	{
		path_to_pt.push_back(list[i]);
	}
}

void PATROL_STATE::SetCurrsmallPoint()
{
	if (path_to_pt.size() == 0)
	{
		std::cout << "No Points of Patrol" << std::endl;
	}
	else
	{
		_currspt = 0;

		if (path_to_pt.size() == 1)
			_nextspt = _currspt;
		else
		{
			if (_currspt + 1 == path_to_pt.size())
				_nextspt = 0;
			else
				_nextspt = _currspt + 1;
		}
	}
}

void PATROL_STATE::GetNextsmallPoint()
{
	_UnitPos = GetOwner()->has(Transform)->GetPosition();

	//std::cout << _UnitPos.x << ", " << _UnitPos.y << " || " << path_to_pt[_currspt].x << ", " << path_to_pt[_currspt].y << " || " << path_to_pt[_nextspt].x << ", " << path_to_pt[_nextspt].y << " || " << path_to_pt.size() << std::endl;

	if (_UnitPos == path_to_pt[_currspt])
	{
		_currspt = _nextspt;

		if (path_to_pt.size() == 1)
			_nextspt = _currspt;
		else
		{
			if (_currspt + 1 == path_to_pt.size())
				_nextspt = 0;
			else
				_nextspt = _currspt + 1;
		}
	}
}

void PATROL_STATE::ChangeDirection(const std::shared_ptr<SpriteComponent>& sprPtr)
{
	sprPtr;
	if ((patrol_pts[_currPt].getx() > patrol_pts[_currPt].getx()) && !isMovingRight)
	{
		isMovingRight = true;
		//sprPtr->FlipSprite();
	}
	else
	{
		isMovingRight = false;
		//sprPtr->FlipSprite();
	}
}

