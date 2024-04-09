/************************************************************************************ /
/*
File Name:		ScriptButtonBack.h
Project Name:	WhereGotLate
Author(s):		Tay Qin Wen Alvin [100%]


Brief:			Handle interaction for back button.


Copyright Information:
All content © 2019 DigiPen (SINGAPORE) Corporation, all rights reserved.
*/
/*************************************************************************************/

#pragma once
#include "ScriptButton.h"


class ScriptButtonBack :	public ScriptButton
{
	bool transition;

public:
	ScriptButtonBack();
	~ScriptButtonBack();


	void OnRelease() override;
	void OnHold() override;
	void CollisionResponse() override;
};

