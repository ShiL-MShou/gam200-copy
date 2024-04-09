/*************************************************************************************/
/*
File Name:		ScriptButtonNo.h
Project Name:	WhereGotLate
Author(s):		Kok Zhe Yan			 [100%]


Copyright Information:
All content © 2019 DigiPen (SINGAPORE) Corporation, all rights reserved.
*/
/*************************************************************************************/
#pragma once
#include "ScriptButton.h"


class ScriptButtonNo : public ScriptButton
{

public:
	void OnRelease() override;
	void OnHold() override;
	void CollisionResponse() override;
};

