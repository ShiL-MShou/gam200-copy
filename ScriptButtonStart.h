/************************************************************************************ /
/*
File Name:		ScriptButtonStart.h
Project Name:	WhereGotLate
Author(s):		Tay Qin Wen Alvin [100%]


Brief:			Handle interaction for start button.


Copyright Information:
All content © 2019 DigiPen (SINGAPORE) Corporation, all rights reserved.
*/
/*************************************************************************************/


#pragma once
#include "ScriptButton.h"


class ScriptButtonStart : public ScriptButton
{

public:
	void OnRelease() override;

	void LoadResource() override;
};

