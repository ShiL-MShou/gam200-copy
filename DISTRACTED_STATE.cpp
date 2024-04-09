/******************************************************************************/
/*
	File Name: DISTRACTED_STATE.cpp
	Project Name: WhereGotLate
	Author(s): Lee Shi Lin [100%]

	Brief: Derived state for teacher NPC

	Copyright Information:
	All content © 2019 DigiPen (SINGAPORE) Corporation, all rights reserved.
*/
/******************************************************************************/
#include "stdafx.h"
#include "DISTRACTED_STATE.h"
#include "StateMachine.h"
#include "SpriteComponent.h"
#include "Logic.h"
#include "FModSoundManager.h"
#include "PathFind.h"
#include "Body.h"


DISTRACTED_STATE::DISTRACTED_STATE()
	: Destination{}, path_to_des{}, _currPt{ 0 }, _nextPt{ 0 }, _UnitPos{}, _pausetime{ 15.0f }, _currtime{ 0.0f }, speed{100.0f}, soundPlayed{false}
{
	iState::_name = "Distracted";
}

DISTRACTED_STATE::~DISTRACTED_STATE()
{
	soundPlayed = false;
}
void DISTRACTED_STATE::Update(float _dt)
{
	//std::cout << "Distracted State" << std::endl;


	if (IsUnableToChase() == true)
		return;

	// Move towards distracted point
	//float step = speed * _dt;

	
	_UnitPos = GetOwner()->has(Transform)->GetPosition();

	if (path_to_des.size() != 0 && _UnitPos != /*Destination*/ path_to_des[path_to_des.size() - 1])
	{
		MoveToPos(GetOwner()->has(Body), _UnitPos, path_to_des[_currPt]);
		NextPoint();
		//MoveTo(step);
	}
	else {
		_currtime += _dt;
		if (!soundPlayed)
		{
			MyEvent.DeActivateAlarmVFX();
			SOUNDMGR->PauseSoundFromMap("alarm");
			SOUNDMGR->setForcePause("alarm", true);
			//if (SOUNDMGR->GetPauseStatus("BGM") == true)
			SOUNDMGR->UnPauseSoundFromMap("BGM");
			SOUNDMGR->setForcePause("BGM", false);

			soundPlayed = true;
		}

		//LOGIC->CanDistract();
		std::shared_ptr<SpriteComponent> sprPtr = GetOwner()->has(SpriteComponent);

		if (sprPtr != nullptr || IsUnableToChase() == false)
		{
			sprPtr->ChangeGenericAnimation("IDLE");
			_ownerSM->updatedSprite = true;
			//if (GetOwner()->has(Transform)->GetScale().x < 0)
			//	sprPtr->FlipSprite();

		}
		if (_currtime >= _pausetime)
		{
			/* Stop moving after Reaching Destination */
			_currtime = 0.0f;
			//std::cout << "Times up & reach destination" << std::endl;
			dynamic_cast<ReachItem*>(GetRawTransitionIndex("stopDistract"))->FinishWaiting();
			//dynamic_cast<ReachItem*>(GetRawTransitionIndex(0))->FinishWaiting();
		}
	}
}

void DISTRACTED_STATE::OnEnter()
{
	iState* s = GetSMOwner()->GetPrevState().get();
	IDLE_STATE* IS;
	PATROL_STATE* PS;
	if ((IS = dynamic_cast<IDLE_STATE*>(s)) != NULL)
	{
		Destination = IS->GetDistractDes();
	}
	if ((PS = dynamic_cast<PATROL_STATE*>(s)) != NULL)
	{
		Destination = PS->GetDistractDes();
	}

	//AddPathPoint(Destination);
	// Set path to des
	_UnitPos = GetOwner()->has(Transform)->GetPosition();
	std::vector<VEC2> g = FindPath(LOGIC->GetAImap(), _UnitPos, Destination);
	
	/*for (size_t i = 0; i < g.size(); i++)
	{
		g[i].printVEC2();
	}*/
	SetPathtoDes(g);
	SetCurrPoint();

	std::shared_ptr<SpriteComponent> sprPtr = GetOwner()->has(SpriteComponent);

	if (sprPtr != nullptr|| IsUnableToChase() == false)
	{
		sprPtr->ChangeGenericAnimation("WALKING");
		_ownerSM->updatedSprite = true;
		//sprPtr->SetCurrSubTextureName("WALKING");
		//sprPtr->SetMaxFrame(15);
		//sprPtr->SetCurrFrame(0);
	}
}

void DISTRACTED_STATE::OnExit()
{
	LOGIC->CanDistract();
	soundPlayed = false;
}

void DISTRACTED_STATE::ResetState()
{
	path_to_des.clear();
}

void DISTRACTED_STATE::UpdateAnimation()
{
	if (IsUnableToChase() == true)
		return;

	GetOwner()->has(SpriteComponent)->ChangeGenericAnimation("WALKING");
}

void DISTRACTED_STATE::AddPathPoint(VEC2 v)
{
	path_to_des.push_back(v);
}

void DISTRACTED_STATE::SetPathtoDes(std::vector<VEC2> list)
{
	for (size_t i = 0; i < list.size(); i++)
	{
		path_to_des.push_back(list[i]);
	}
}

void DISTRACTED_STATE::SetCurrPoint()
{
	if (path_to_des.size() == 0)
	{
		std::cout << "No Points of Patrol" << std::endl;
	}
	else
	{
		_currPt = 0;

		if (path_to_des.size() == 1)
			_nextPt = _currPt;
		else
		{
			if (_currPt + 1 == path_to_des.size())
				_nextPt = 0;
			else
				_nextPt = _currPt + 1;
		}
	}
}

void DISTRACTED_STATE::NextPoint()
{
	_UnitPos = GetOwner()->has(Transform)->GetPosition();

	if (_UnitPos == path_to_des[_currPt])
	{
		_currPt = _nextPt;

		if (path_to_des.size() == 1)
			_nextPt = _currPt;
		else
		{
			if (_currPt + 1 == path_to_des.size())
				_nextPt = 0;
			else
				_nextPt = _currPt + 1;
		}
	}
}

void DISTRACTED_STATE::MoveTo(float step)
{
	UNREFERENCED_PARAMETER(step);

	_UnitPos = GetOwner()->has(Transform)->GetPosition();
	//======================
	// OLD move to position
	//======================
	//VEC2 n = VEC2MoveTowards(_UnitPos, path_to_des[_currPt], step);
	//GetOwner()->has(Transform)->SetPosition(n);

	//======================
	// NEW move to position
	//======================
	// Snap to location
	if (VEC2Magnitude(_UnitPos, path_to_des[_currPt]) <= 1)
	{
		//std::cout << "Snap" << std::endl;
		GetOwner()->has(Transform)->SetPosition(path_to_des[_currPt]);

		GetOwner()->has(Body)->DeactivateForces("playerRight");
		GetOwner()->has(Body)->DeactivateForces("playerLeft");
		GetOwner()->has(Body)->DeactivateForces("playerUp");
		GetOwner()->has(Body)->DeactivateForces("playerDown");
		GetOwner()->has(Body)->DeactivateForces("drag");

		GetOwner()->has(Body)->SetVelocity(VEC2(0, 0));

		NextPoint();
		return;
	}

	if (_UnitPos.x < path_to_des[_currPt].x) // move right
	{
		//std::cout << "right" << _UnitPos.y << std::endl;
		GetOwner()->has(Body)->ActivateForces("playerRight");
		GetOwner()->has(Body)->ActivateForces("drag");
	}
	else if (_UnitPos.x > path_to_des[_currPt].x) // move left
	{
		//std::cout << "left " << _UnitPos.y << std::endl;
		GetOwner()->has(Body)->ActivateForces("playerLeft");
		GetOwner()->has(Body)->ActivateForces("drag");
	}

	if (_UnitPos.y < path_to_des[_currPt].y) // move up
	{
		//std::cout << "Up " << _UnitPos.y << std::endl;
		GetOwner()->has(Body)->ActivateForces("playerUp");
		GetOwner()->has(Body)->ActivateForces("drag");
	}
	else if (_UnitPos.y > path_to_des[_currPt].y) // move down
	{
		//std::cout << "Down" << _UnitPos.y << std::endl;
		GetOwner()->has(Body)->ActivateForces("playerDown");
		GetOwner()->has(Body)->ActivateForces("drag");
	}

	// update facing direction

	//if (_UnitPos.x < n.x && GetOwner()->has(Transform)->GetScale().x < 0)
	//{
	//	GetOwner()->has(SpriteComponent)->FlipSprite();
	//}
	//else if (_UnitPos.x > n.x && GetOwner()->has(Transform)->GetScale().x > 0)
	//{
	//	GetOwner()->has(SpriteComponent)->FlipSprite();
	//}
	
	NextPoint();
}
