/************************************************************************************ /
/*
File Name:		WaterBottleScript.h
Project Name:	WhereGotLate
Author(s):		Tay Qin Wen Alvin [100%]

Brief:			Script for water bottle. (NOT USED)


Copyright Information:
All content © 2020 DigiPen (SINGAPORE) Corporation, all rights reserved.
*/
/*************************************************************************************/

#include "stdafx.h"
#include "WaterBottleScript.h"

void WaterBottleScript::Init()
{
	waterGauge = 0.0;
}

void WaterBottleScript::Start()
{
}

void WaterBottleScript::Update(float _dt)
{
	UNREFERENCED_PARAMETER(_dt);

	waterGauge = static_cast<float>(sprPtr->GetCurrFrameNum());
}

void WaterBottleScript::Exit()
{
}

void WaterBottleScript::SetOwner(std::shared_ptr<GOC> _gameObjPtr)
{
	gameObjPtr = _gameObjPtr;
	sprPtr = _gameObjPtr->has(SpriteComponent);
}

void WaterBottleScript::LoadResource()
{

}
