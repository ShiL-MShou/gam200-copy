/*************************************************************************************/
/*
File Name:		ConfirmQuitScript.h
Project Name:	WhereGotLate
Author(s):		Kok Zhe Yan			 [100%]


Copyright Information:
All content © 2019 DigiPen (SINGAPORE) Corporation, all rights reserved.
*/
/*************************************************************************************/
#pragma once

#include "Script.h"

class ConfirmQuitScript : public Script
{
	std::vector<GOCId> mObjList;

	bool isActive;

public:
	ConfirmQuitScript() : isActive{ false } {}

	virtual void Start() override;
	virtual void Update(float _dt) override;
	virtual void Exit() override;

	virtual void LoadResource() override;

	void Init() override;


	virtual void DestroyMenu();

};
