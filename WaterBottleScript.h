/************************************************************************************ /
/*
File Name:		WaterBottleScript.cpp
Project Name:	WhereGotLate
Author(s):		Tay Qin Wen Alvin [100%]

Brief:			Script for water bottle. (NOT USED)


Copyright Information:
All content © 2020 DigiPen (SINGAPORE) Corporation, all rights reserved.
*/
/*************************************************************************************/

#pragma once
#include "Script.h"

#include <memory>
#include "SpriteComponent.h"


class WaterBottleScript :	public Script
{
	float waterGauge;
	std::shared_ptr<SpriteComponent> sprPtr;


public:
	void Init() override;

	void Start() override;

	void Update(float _dt) override;

	void Exit() override;

	void SetOwner(std::shared_ptr<GOC> _gameObjPtr) override;

	void LoadResource() override;
};

