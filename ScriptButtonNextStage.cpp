/************************************************************************************ /
/*
File Name:		ScriptButtonNextStage.cpp
Project Name:	WhereGotLate
Author(s):		Tay Qin Wen Alvin [100%]


Brief:			Handle interaction for next stage button.


Copyright Information:
All content © 2019 DigiPen (SINGAPORE) Corporation, all rights reserved.
*/
/*************************************************************************************/

#include "stdafx.h"
#include "ScriptButtonNextStage.h"
#include "FModSoundManager.h"

// to change the stage
#include "Logic.h"

#include "TransitionScript.h"


void ScriptButtonNextStage::Update(float _dt)
{
	if (CORE->GetPausedStatus())
	{
		if (selectedPtr.lock() != nullptr)
			selectedPtr.lock()->Destroy();
		return;
	}
	else
		ScriptButton::Update(_dt);
}

void ScriptButtonNextStage::OnRelease()
{
	SOUNDMGR->PauseSoundFromMap("winMusic");
	SOUNDMGR->setForcePause("winMusic", true);
	SOUNDMGR->setForcePause("Nag", true);

	if (strcmp("Stage1v2", CORE->GetCurrLevel().c_str()) == 0)
	{
		// LOGIC->ChangeLevel("Stage2v2");
		std::shared_ptr<GOC> transitionPtr = LOGIC->CreateObjectAt(VEC2(640, 360), 0, "Transition", "Transition", true);

		LOGIC->GetScript<TransitionScript>("TRANSITION_SCRIPT")->SetTransition("Stage2v2");
	}
	else if (strcmp("Stage2v2", CORE->GetCurrLevel().c_str()) == 0)
	{
		// LOGIC->ChangeLevel("Stage3v2");
		std::shared_ptr<GOC> transitionPtr = LOGIC->CreateObjectAt(VEC2(640, 360), 0, "Transition", "Transition", true);

		LOGIC->GetScript<TransitionScript>("TRANSITION_SCRIPT")->SetTransition("Stage3v2");
	}
	else if (strcmp("Stage3v2", CORE->GetCurrLevel().c_str()) == 0)
	{
		//LOGIC->ChangeLevel("Stage4");

		std::shared_ptr<GOC> transitionPtr = LOGIC->CreateObjectAt(VEC2(640, 360), 0, "Transition", "Transition", true);

		LOGIC->GetScript<TransitionScript>("TRANSITION_SCRIPT")->SetTransition("Stage4");
	}	
	else if (strcmp("Stage4", CORE->GetCurrLevel().c_str()) == 0)
	{
		//LOGIC->ChangeLevel("Result");

		std::shared_ptr<GOC> transitionPtr = LOGIC->CreateObjectAt(VEC2(640, 360), 0, "Transition", "Transition", true);

		LOGIC->GetScript<TransitionScript>("TRANSITION_SCRIPT")->SetTransition("Result");
	}
	else
	{
		// LOGIC->ChangeLevel("MainMenu");
		std::shared_ptr<GOC> transitionPtr = LOGIC->CreateObjectAt(VEC2(640, 360), 0, "Transition", "Transition", true);

		LOGIC->GetScript<TransitionScript>("TRANSITION_SCRIPT")->SetTransition("MainMenu");
	}	
}
