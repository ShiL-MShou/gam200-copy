/************************************************************************************ /
/*
File Name:		ScriptButtonStart.cpp
Project Name:	WhereGotLate
Author(s):		Tay Qin Wen Alvin [100%]


Brief:			Handle interaction for start button.


Copyright Information:
All content © 2019 DigiPen (SINGAPORE) Corporation, all rights reserved.
*/
/*************************************************************************************/


#include "stdafx.h"
#include "ScriptButtonStart.h"
#include "FModSoundManager.h"
#include "MainMenuScript.h"
// to change level
#include "Logic.h"
#include "TransitionScript.h"



void ScriptButtonStart::OnRelease()
{
	//LOGIC->ChangeLevel("Stage1v2");

	std::shared_ptr<GOC> transitionPtr = LOGIC->CreateObjectAt(VEC2(640, 360), 0, "Transition", "Transition", true);
	
	LOGIC->GetScript<TransitionScript>("TRANSITION_SCRIPT")->SetTransition("ComicScreen");
	
	//std::shared_ptr<GOC> comicPtr = LOGIC->CreateObjectAt(VEC2(640, 360), 0, "ComicWLogic", "ComicWLogic", false);


	SOUNDMGR->PauseAllSounds();
	notInGame = false;
}

void ScriptButtonStart::LoadResource()
{
	// loading resources
	FACTORY->CreateArcheType("SelectedCircle");
	FACTORY->CreateArcheType("Transition");

	FACTORY->CreateArcheType("ComicWLogic");
}

