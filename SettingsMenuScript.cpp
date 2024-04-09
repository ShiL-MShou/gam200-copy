/*************************************************************************************/
/*
File Name:		SettingsMenuScript.cpp
Project Name:	WhereGotLate
Author(s):		Kok Zhe Yan			 [100%]


Copyright Information:
All content © 2019 DigiPen (SINGAPORE) Corporation, all rights reserved.
*/
/*************************************************************************************/
#include "stdafx.h"

#include "SettingsMenuScript.h"

#include "Logic.h"
#include "Graphics.h"
#include "Input.h"
#include "CoreEngine.h"
#include "FModSoundManager.h"
#include "MainMenuScript.h"//extern bool notIngame
#include "Input.h"


void SettingsMenuScript::LoadResource()
{
}

void SettingsMenuScript::Start()
{
	//pausedMusic = true;
}

void SettingsMenuScript::Update(float _dt)
{
	UNREFERENCED_PARAMETER(_dt);
	if (notInGame)
	{
		SOUNDMGR->UnPauseSoundFromMap("menuMusic");
	}

	GRAPHICS->GetTextRenderer()->AddText(std::to_string(static_cast<int>(roundf(SOUNDMGR->GetMasterVolume() * 10) * 10)), VEC2(640.0f, 240.0f));
	GRAPHICS->GetTextRenderer()->AddText("FULLSCREEN", VEC2(550, 440.0f));
	GRAPHICS->GetTextRenderer()->AddText("MUTE", VEC2(600.0f, 340.0f));
}

void SettingsMenuScript::Exit()
{

}
