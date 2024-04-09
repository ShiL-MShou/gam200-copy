/************************************************************************************ /
/*
File Name:		ScriptButtonQuit.h
Project Name:	WhereGotLate
Author(s):		Tay Qin Wen Alvin [100%]


Brief:			Handle interaction for quit button.


Copyright Information:
All content © 2019 DigiPen (SINGAPORE) Corporation, all rights reserved.
*/
/*************************************************************************************/

#pragma once
#include "ScriptButton.h"


class ScriptButtonQuit : public ScriptButton
{
public:
	void Update(float _dt) override;

	void OnRelease() override;
};

