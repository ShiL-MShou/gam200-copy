/*****************************************************************************/
/*
File Name:		ScriptButtonMainMenu.cpp
Project Name:	WhereGotLate
Author(s):		Ong Cheng Kai Marcus [50%]
				Tay Qin Wen Alvin    [50%]

Copyright information:
All content copyright 2020 DigiPen (SINGAPORE) Corporation, all rights reserved.
*/
/*****************************************************************************/
#include "stdafx.h"
#include "ScriptButtonMainMenu.h"

// to change the level
#include "Logic.h"
#include "FModSoundManager.h"
#include "Score.h"
#include "TransitionScript.h"


void ScriptButtonMainMenu::Update(float _dt)
{
	if (CORE->GetPausedStatus() && gameObjPtr.lock()->has(LogicComponent)->GetButtonType() != LogicComponent::BUTTONTYPE::PAUSEMENU)
		return;
	if (MyEvent.inConfirmQuit)
	{
		if (selectedPtr.lock() != nullptr)
			selectedPtr.lock()->Destroy();
		return;
	}
	else
		ScriptButton::Update(_dt);
}

void ScriptButtonMainMenu::OnRelease()
{
	SOUNDMGR->PauseAllSounds();
	SOUNDMGR->setForcePause("BGM", true);
	MyScore.clearAllStageVars();

	//LOGIC->ChangeLevel("MainMenu");

	std::shared_ptr<GOC> transitionPtr = LOGIC->CreateObjectAt(VEC2(640, 360), 0, "Transition", "Transition", true);
	LOGIC->GetScript<TransitionScript>("TRANSITION_SCRIPT")->SetTransition("MainMenu");


	SOUNDMGR->PlaySoundFromMap("menuMusic");
}
