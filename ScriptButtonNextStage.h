/************************************************************************************ /
/*
File Name:		ScriptButtonNextStage.h
Project Name:	WhereGotLate
Author(s):		Tay Qin Wen Alvin [100%]


Brief:			Handle interaction for next stage button.


Copyright Information:
All content © 2019 DigiPen (SINGAPORE) Corporation, all rights reserved.
*/
/*************************************************************************************/

#pragma once
#include "ScriptButton.h"


class ScriptButtonNextStage : public ScriptButton
{

public:
	void Update(float _dt) override;

	void OnRelease() override;
};

