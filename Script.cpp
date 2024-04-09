/************************************************************************************ /
/*
File Name:		Script.h
Project Name:	WhereGotLate
Author(s):		Tay Qin Wen Alvin [100%]


Brief:			Base for all the scripts


Copyright Information:
All content © 2019 DigiPen (SINGAPORE) Corporation, all rights reserved.
*/
/*************************************************************************************/

#include "stdafx.h"
#include "Script.h"


void Script::Start()
{

}

void Script::Init()
{
}

void Script::Update(float _dt)
{
	UNREFERENCED_PARAMETER(_dt);
}

void Script::Exit()
{
}

void Script::SetOwner(std::shared_ptr<GOC> _gameObjPtr)
{
	gameObjPtr = _gameObjPtr;
}

std::weak_ptr<GOC> Script::GetOwner()
{
	return gameObjPtr = gameObjPtr;
}

void Script::UseItem(const VEC2& _pos, int _frame)
{
	UNREFERENCED_PARAMETER(_pos);
	UNREFERENCED_PARAMETER(_frame);
}

void Script::CollisionResponse(CollideTypes _type, VEC2 _aabbMin, VEC2 _aabbMax)
{
	UNREFERENCED_PARAMETER(_type);
	UNREFERENCED_PARAMETER(_aabbMin);
	UNREFERENCED_PARAMETER(_aabbMax);
}

void Script::LoadResource()
{
}
