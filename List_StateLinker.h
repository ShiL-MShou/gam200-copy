/******************************************************************************/
/*
	File Name: List_StateLinker.h
	Project Name: WhereGotLate
	Author(s): Lee Shi Lin [100%]

	Brief: Collection of Transitions between States

	Copyright Information:
	All content © 2019 DigiPen (SINGAPORE) Corporation, all rights reserved.
*/
/******************************************************************************/
#pragma once
#include "Transition.h"
#include "Transform.h"
#include "IDLE_STATE.h"
#include "PATROL_STATE.h"

/********************************************/
// List of different linkers between States
/********************************************/

/* Idle to Patrol */
class TimerTransition : public iTransition
{
	
	float _currtime = 0.0f;
public:
	TimerTransition(int _stateto) : iTransition(_stateto) {};
	bool Condition(float _dt) override {
		_currtime += _dt;
		if (_currtime >= _time)
		{
			_currtime = 0.0f;
			return true;
		}
		return false;
	}

	float _time = 6.0f;
};

/* Patrol to Idle */
class Rest : public iTransition
{
public:
	Rest(int _stateto) : iTransition(_stateto) {};
	bool Condition(float _dt) override
	{
		UNREFERENCED_PARAMETER(_dt);
		// If UnitPos == currPt
		PATROL_STATE* check = dynamic_cast<PATROL_STATE*>(GetStateOwner());
		VEC2 UPos = check->GetUnitPos();
		VEC2 cPos = check->GetCurrPt();

		//std::cout << check->_currPt << " : " << UPos.x << " " << UPos.y << " |  " << cPos.x << " " << cPos.y << std::endl;

		if (UPos == cPos)
		{
			return true;
		}
		return false;
	}
};

/* Distracted to Patrol */
class ReachItem : public iTransition
{
	bool fw = false;
public:
	ReachItem(int _stateto) : iTransition(_stateto) {};
	bool Condition(float _dt) override
	{
		UNREFERENCED_PARAMETER(_dt);
		// If item pos == unit pos
		// start timer

		// if timer and position same
		// set curr time = 0
		// return true
		if (fw)
		{
			fw = false;
			return true;
		}
		return fw;
	}

	void FinishWaiting()
	{
		fw = true;
	}

};

/* Idle + Patrol to Distracted */
class Distracted : public iTransition
{
	bool detect = false;
	VEC2 destination;
public:
	Distracted(int _stateto) : iTransition(_stateto) {};
	bool Condition(float _dt) override
	{
		UNREFERENCED_PARAMETER(_dt);
		IDLE_STATE* IS;
		PATROL_STATE* PS;
		if ((IS = dynamic_cast<IDLE_STATE*>(GetStateOwner())) != NULL)
		{
			IS->SetDistractDes(destination);
		}
		if ((PS = dynamic_cast<PATROL_STATE*>(GetStateOwner())) != NULL)
		{
			PS->SetDistractDes(destination);
		}

		if (detect)
		{
			detect = false;
			return true;
		}
		return detect;
	}

	/* Check world if any distraction spawned */
	void DistractMsg(bool i, VEC2 target)
	{
		detect = i;
		destination = target;
	}

};

// Any state to chase
class DetectPlayer : public iTransition
{
	bool seeplayer = false;
public:
	DetectPlayer(int _stateto) : iTransition(_stateto) {};
	bool Condition(float _dt) override
	{
		UNREFERENCED_PARAMETER(_dt);
		// grid to detect
		if (seeplayer)
		{
			seeplayer = false;
			return true;
		}
		return seeplayer;
	}

	void FoundPlayer()
	{
		seeplayer = true;
	}
};

/* Chase to Patrol */
class GiveUpChasing : public iTransition
{
	bool stun = false;
public:
	GiveUpChasing(int _stateto) : iTransition(_stateto) {};
	bool Condition(float _dt) override
	{
		UNREFERENCED_PARAMETER(_dt);
		// grid to detect
		if (stun)
		{
			stun = false;
			return true;
		}
		return stun;
	}

	void Giveup()
	{
		stun = true;
	}
};