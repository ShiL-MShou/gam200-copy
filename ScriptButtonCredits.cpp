/************************************************************************************ /
/*
File Name:		ScriptButtonCredits.h
Project Name:	WhereGotLate
Author(s):		Tay Qin Wen Alvin [100%]


Brief:			Handle interaction for credits button.


Copyright Information:
All content © 2019 DigiPen (SINGAPORE) Corporation, all rights reserved.
*/
/*************************************************************************************/

#include "stdafx.h"
#include "ScriptButtonCredits.h"

// to change level
#include "Logic.h"

#include "TransitionScript.h"


void ScriptButtonCredits::OnRelease()
{
	//LOGIC->ChangeLevel("Credits");

	std::shared_ptr<GOC> transitionPtr = LOGIC->CreateObjectAt(VEC2(640, 360), 0, "Transition", "Transition", true);
	//TransitionScript ts = transitionPtr->has(LogicComponent)->GetScriptName

	LOGIC->GetScript<TransitionScript>("TRANSITION_SCRIPT")->SetTransition("Credits");

}

