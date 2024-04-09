/************************************************************************************ /
/*
File Name:		ScriptButtonHowToPlay.cpp
Project Name:	WhereGotLate
Author(s):		Tay Qin Wen Alvin [100%]


Brief:			Handle interaction for how to play button.


Copyright Information:
All content © 2019 DigiPen (SINGAPORE) Corporation, all rights reserved.
*/
/*************************************************************************************/


#include "stdafx.h"
#include "ScriptButtonHowToPlay.h"

// to change level
#include "Logic.h"

#include "TransitionScript.h"


void ScriptButtonHowToPlay::OnRelease()
{
	//LOGIC->ChangeLevel("HowToPlay");

	std::shared_ptr<GOC> transitionPtr = LOGIC->CreateObjectAt(VEC2(640, 360), 0, "Transition", "Transition", true);

	LOGIC->GetScript<TransitionScript>("TRANSITION_SCRIPT")->SetTransition("HowToPlay");
}
