/************************************************************************************ /
/*
File Name:		ScriptButtonSetting.cpp
Project Name:	WhereGotLate
Author(s):		Tay Qin Wen Alvin [100%]


Brief:			Handle interaction for setting button.


Copyright Information:
All content © 2019 DigiPen (SINGAPORE) Corporation, all rights reserved.
*/
/*************************************************************************************/

#include "stdafx.h"
#include "ScriptButtonSettings.h"

// to change level
#include "Logic.h"
#include "TransitionScript.h"


void ScriptButtonSettings::OnRelease()
{
	//LOGIC->ChangeLevel("Settings");

	std::shared_ptr<GOC> transitionPtr = LOGIC->CreateObjectAt(VEC2(640, 360), 0, "Transition", "Transition", true);

	LOGIC->GetScript<TransitionScript>("TRANSITION_SCRIPT")->SetTransition("Settings");
}
