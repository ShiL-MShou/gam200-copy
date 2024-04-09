/************************************************************************************ /
/*
File Name:		WaterScript.h
Project Name:	WhereGotLate
Author(s):		Tay Qin Wen Alvin [100%]

Brief:			Script for water. (NOT USED)


Copyright Information:
All content © 2020 DigiPen (SINGAPORE) Corporation, all rights reserved.
*/
/*************************************************************************************/

#pragma once
#include "Script.h"

#include "Timer.h"


class WaterScript : public Script
{
	// timer
	//Timer timer;

public:
	WaterScript();
	~WaterScript();

	WaterScript(const WaterScript&) = delete;
	WaterScript& operator=(const WaterScript&) = delete;


	void Init() override;

	void Start() override;

	void Update(float _dt) override;

	void Exit() override;

	//void SetOwner(std::shared_ptr<GOC> _gameObjPtr) override;

	void LoadResource() override;

	bool IsValid();
};

