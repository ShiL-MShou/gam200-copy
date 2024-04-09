/*************************************************************************************/
/*
File Name:		PauseMenuScript.cpp
Project Name:	WhereGotLate
Author(s):		Kok Zhe Yan			 [100%]


Copyright Information:
All content © 2019 DigiPen (SINGAPORE) Corporation, all rights reserved.
*/
/*************************************************************************************/
#include "stdafx.h"

#include "PauseMenuScript.h"

#include "Logic.h"
#include "Graphics.h"
#include "Input.h"
#include "CoreEngine.h"
#include "FModSoundManager.h"


void PauseMenuScript::LoadResource()
{
	FACTORY->CreateArcheType("PauseMenuBG");
	FACTORY->CreateArcheType("ButtonResumeGame");
	FACTORY->CreateArcheType("PauseButtonMainMenu");
	FACTORY->CreateArcheType("ButtonQuitGame");
	FACTORY->CreateArcheType("ButtonQuitGamePause");
}

void PauseMenuScript::Start()
{
}

void PauseMenuScript::Update(float _dt)
{
	UNREFERENCED_PARAMETER(_dt);

	if (isActive)
	{
		GRAPHICS->GetTextRenderer()->AddText(std::to_string(static_cast<int>(roundf(SOUNDMGR->GetMasterVolume() * 10) * 10)), VEC2(830.0f, 240.0f));
		GRAPHICS->GetTextRenderer()->AddText("FULLSCREEN", VEC2(700.0f, 440.0f));
		GRAPHICS->GetTextRenderer()->AddText("MUTE", VEC2(740.0f, 340.0f));
	}


	if (INPUTMGR->GetKeyRelease(MVK_ESCAPE))
	{
		if (isActive)
		{
			DestroyMenu();

		}
		else
		{
			CreateMenu();
		}
	}
}

void PauseMenuScript::Exit()
{
	isActive = false;

}

void PauseMenuScript::CreateMenu()
{
	isActive = true;
	CORE->SetPausedStatus(true);
	MyEvent.inGameStage = false;

	std::shared_ptr<GOC> pausebg = LOGIC->CreateObjectAt(VEC2{}, 0.0f, "PauseMenuBG", "PauseMenuBG");
	//pausebg->has(Sprite)->SetStartFrame(4);
	//pausebg->has(Sprite)->SetCurrFrame(4);
	pausebg->has(SpriteComponent)->SetFixedPosition(VEC2{ 750.0f, 350.0f });
	mObjlist.push_back(pausebg->GetId());

	std::shared_ptr<GOC> fscheckbox = LOGIC->CreateObjectAt(VEC2{}, 0.0f, "FullScreenCheckBox", "FullScreenCheckBox");
	fscheckbox->has(SpriteComponent)->SetStartFrame(4);
	fscheckbox->has(SpriteComponent)->SetCurrFrame(5);
	fscheckbox->has(SpriteComponent)->SetFixedPosition(VEC2{ 900.0f, 450.0f });
	mObjlist.push_back(fscheckbox->GetId());

	std::shared_ptr<GOC> mutecheckbox = LOGIC->CreateObjectAt(VEC2{}, 0.0f, "MuteCheckBox", "MuteCheckBox");
	mutecheckbox->has(SpriteComponent)->SetStartFrame(4);
	mutecheckbox->has(SpriteComponent)->SetCurrFrame(4);
	mutecheckbox->has(SpriteComponent)->SetFixedPosition(VEC2{ 900.0f, 350.0f });
	mObjlist.push_back(mutecheckbox->GetId());

	std::shared_ptr<GOC> volumeup = LOGIC->CreateObjectAt(VEC2{}, 0.0f, "VolumeUp", "VolumeUp");
	//volumeup->has(Sprite)->SetStartFrame(0);
	//volumeup->has(Sprite)->SetCurrFrame(0);
	volumeup->has(SpriteComponent)->SetFixedPosition(VEC2{ 950.0f, 250.0f });
	mObjlist.push_back(volumeup->GetId());

	std::shared_ptr<GOC> volumedown = LOGIC->CreateObjectAt(VEC2{}, 0.0f, "VolumeDown", "VolumeDown");
	volumedown->has(SpriteComponent)->SetStartFrame(3);
	volumedown->has(SpriteComponent)->SetCurrFrame(3);
	volumedown->has(SpriteComponent)->SetFixedPosition(VEC2{ 750.0f, 250.0f });
	mObjlist.push_back(volumedown->GetId());

	std::shared_ptr<GOC> resume = LOGIC->CreateObjectAt(VEC2{}, 0.0f, "ButtonResumeGame", "PauseResumeGame");
	resume->has(SpriteComponent)->SetStartFrame(16);
	resume->has(SpriteComponent)->SetCurrFrame(16);
	resume->has(SpriteComponent)->SetFixedPosition(VEC2{ 400.0f, 450.0f });
	mObjlist.push_back(resume->GetId());


	std::shared_ptr<GOC> mainmenu = LOGIC->CreateObjectAt(VEC2{}, 0.0f, "PauseButtonMainMenu", "PauseaMainMenu");
	mainmenu->has(LogicComponent)->SetButtonType(LogicComponent::BUTTONTYPE::PAUSEMENU);
	mainmenu->has(SpriteComponent)->SetStartFrame(12);
	mainmenu->has(SpriteComponent)->SetCurrFrame(12);
	mainmenu->has(SpriteComponent)->SetFixedPosition(VEC2{ 400.0f, 350.0f });
	mObjlist.push_back(mainmenu->GetId());

	std::shared_ptr<GOC> quit = LOGIC->CreateObjectAt(VEC2{}, 0.0f, "ButtonQuitGamePause", "PauseQuitGame");
	quit->has(SpriteComponent)->SetStartFrame(18);
	quit->has(SpriteComponent)->SetCurrFrame(18);
	quit->has(SpriteComponent)->SetFixedPosition(VEC2{ 400.0f, 250.0f });
	mObjlist.push_back(quit->GetId());

	std::shared_ptr<GOC> tint = LOGIC->CreateObjectAt(VEC2{}, 0.0f, "Tint", "PauseTint");
	mObjlist.push_back(tint->GetId());
}

void PauseMenuScript::DestroyMenu()
{
	isActive = false;
	CORE->SetPausedStatus(false);
	MyEvent.inGameStage = true;
	for (GOCId id : mObjlist)
	{
		MyResource.RemoveObject(id);
	}
	mObjlist.clear();
}
