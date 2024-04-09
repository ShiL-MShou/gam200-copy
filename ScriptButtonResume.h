/*************************************************************************************/
/*
File Name:		ScriptButtonResume.h
Project Name:	WhereGotLate
Author(s):		Kok Zhe Yan			 [100%]


Copyright Information:
All content © 2019 DigiPen (SINGAPORE) Corporation, all rights reserved.
*/
/*************************************************************************************/
#pragma once
#include "ScriptButton.h"


class ScriptButtonResume : public ScriptButton
{
public:
	void Update(float _dt) override;

	void OnRelease() override;
};