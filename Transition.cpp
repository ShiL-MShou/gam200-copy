/******************************************************************************/
/*
	File Name: Transition.cpp
	Project Name: WhereGotLate
	Author(s): Lee Shi Lin [100%]

	Brief: Interface class to all transitions between states

	Copyright Information:
	All content © 2019 DigiPen (SINGAPORE) Corporation, all rights reserved.
*/
/******************************************************************************/
#include "stdafx.h"
#include "Composition.h"
#include "Transition.h"
#include "State.h"

bool iTransition::Condition(float _dt)
{
	UNREFERENCED_PARAMETER(_dt);
	return false;
}

int iTransition::GetNextStateNum()
{
	return _ChangeToState;
}

void iTransition::SetOwner(iState* st)
{
	_ownerS = st;
}

std::shared_ptr<GOC> iTransition::GetObjOwner()
{
	return _ownerS->GetOwner();
}

iState* iTransition::GetStateOwner()
{
	return _ownerS;
}
