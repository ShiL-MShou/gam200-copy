/*************************************************************************************/
/*
File Name:		SettingsMenuScript.h
Project Name:	WhereGotLate
Author(s):		Kok Zhe Yan			 [100%]


Copyright Information:
All content © 2019 DigiPen (SINGAPORE) Corporation, all rights reserved.
*/
/*************************************************************************************/
#pragma once

#include "Script.h"

class SettingsMenuScript : public Script
{

	std::vector<GOCId> mObjlist;

	//bool pausedMusic;

public:

	void Start() override;

	void Update(float _dt) override;

	void Exit() override;

	void LoadResource() override;
};
