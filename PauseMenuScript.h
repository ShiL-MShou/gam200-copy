/*************************************************************************************/
/*
File Name:		PauseMenuScript.h
Project Name:	WhereGotLate
Author(s):		Kok Zhe Yan			 [100%]


Copyright Information:
All content © 2019 DigiPen (SINGAPORE) Corporation, all rights reserved.
*/
/*************************************************************************************/
#pragma once

#include "Script.h"

class PauseMenuScript : public Script
{
	bool isActive;

	std::vector<GOCId> mObjlist;


public:
	PauseMenuScript() : isActive{ false } {}

	void Start() override;

	void Update(float _dt) override;

	void Exit() override;

	void LoadResource() override;

	void CreateMenu();

	void DestroyMenu();
};
