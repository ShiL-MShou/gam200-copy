/*****************************************************************************/
/*
File Name:		MainMenuScript.cpp
Project Name:	WhereGotLate
Author(s):		Ong Cheng Kai Marcus [100%]

Copyright information:
All content copyright 2020 DigiPen (SINGAPORE) Corporation, all rights reserved.
*/
/*****************************************************************************/
#include "stdafx.h"
#include "FModSoundManager.h"
#include "MainMenuScript.h"

#include "Input.h"
#include "Logic.h"

bool notInGame;

void MainMenuScript::Start()
{
	//SOUNDMGR->PlaySoundLoopFromMap("menuBGM");
	//SOUNDMGR->UnPauseSoundFromMap("menuBGM");
	playmusic = false;
	notInGame = true;
}

void MainMenuScript::Update(float _dt)
{
	UNREFERENCED_PARAMETER(_dt); 
	if(!playmusic)
	{
		SOUNDMGR->PlaySoundLoopFromMap("menuMusic");
		playmusic = true;
	}
	else if (notInGame)
	{
		SOUNDMGR->UnPauseSoundFromMap("menuMusic");
	}

	//if (INPUTMGR->GetKeyPressed(MVK_RETURN))
		//LOGIC->ChangeLevel("TestSave");
	if (INPUTMGR->GetKeyPressed(MVK_LBUTTON))
		SOUNDMGR->PlaySoundFromMap("click");

	//if (INPUTMGR->GetKeyPressed(MVK_RETURN))
	//{
	//	LOGIC->ChangeLevel("TestSave");
	//	SOUNDMGR->PauseAllSounds();
	//}
}

void MainMenuScript::Exit()
{
	//SOUNDMGR->PauseAllSounds();
	playmusic = false;
}
