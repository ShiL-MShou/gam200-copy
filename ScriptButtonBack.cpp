/************************************************************************************ /
/*
File Name:		ScriptButtonBack.cpp
Project Name:	WhereGotLate
Author(s):		Tay Qin Wen Alvin [100%]


Brief:			Handle interaction for back button.


Copyright Information:
All content © 2019 DigiPen (SINGAPORE) Corporation, all rights reserved.
*/
/*************************************************************************************/

#include "stdafx.h"
#include "ScriptButtonBack.h"

// to change level
#include "Logic.h"
#include "TransitionScript.h"


ScriptButtonBack::ScriptButtonBack()
	: transition {false}
{
}

ScriptButtonBack::~ScriptButtonBack()
{
}

void ScriptButtonBack::OnRelease()
{
	//LOGIC->ChangeLevel("MainMenu");

	std::shared_ptr<GOC> transitionPtr = LOGIC->CreateObjectAt(VEC2(640, 360), 0, "Transition", "Transition", true);
	
	LOGIC->GetScript<TransitionScript>("TRANSITION_SCRIPT")->SetTransition("MainMenu");
}

void ScriptButtonBack::OnHold()
{
}

void ScriptButtonBack::CollisionResponse()
{
}
