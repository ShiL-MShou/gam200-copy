/******************************************************************************/
/*
	File Name: Transition.h
	Project Name: WhereGotLate
	Author(s): Lee Shi Lin [100%]

	Brief: Interface class to all transitions between states

	Copyright Information:
	All content © 2019 DigiPen (SINGAPORE) Corporation, all rights reserved.
*/
/******************************************************************************/
#pragma once

//#include "State.h"
#include "Composition.h"

class iState;

class iTransition
{
	iState* _ownerS;
	int _ChangeToState;
public:
	iTransition(int _stateto) : _ChangeToState{ _stateto } {};
	virtual bool Condition(float _dt);

	int GetNextStateNum();

	void SetOwner(iState* st);

	std::shared_ptr<GOC> GetObjOwner();

	iState* GetStateOwner();
};