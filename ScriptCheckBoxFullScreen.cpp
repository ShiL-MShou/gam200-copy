/*************************************************************************************/
/*
File Name:		ScriptCheckBoxFullScreen.cpp
Project Name:	WhereGotLate
Author(s):		Kok Zhe Yan			 [100%]


Copyright Information:
All content © 2019 DigiPen (SINGAPORE) Corporation, all rights reserved.
*/
/*************************************************************************************/
#include "stdafx.h"
#include "ScriptCheckBoxFullScreen.h"


#include "Logic.h"
#include "WindowsSystem.h"
//#include "Graphics.h"
#include "FModSoundManager.h"

ScriptCheckBoxFullScreen::ScriptCheckBoxFullScreen()
{
}

ScriptCheckBoxFullScreen::~ScriptCheckBoxFullScreen()
{
}

void ScriptCheckBoxFullScreen::Start()
{
}

void ScriptCheckBoxFullScreen::Init()
{
}

void ScriptCheckBoxFullScreen::Update(float _dt)
{
	UNREFERENCED_PARAMETER(_dt);
	if (WINDOWS->isWindowMode)
		spritePtr->SetCurrentFrame(4);

	if (PHYSICS->IsCollidingWithMouse(colliderPtr))
	{
		if (INPUTMGR->GetKeyHold(MVK_LBUTTON))
			OnHold();
		else if (INPUTMGR->GetKeyRelease(MVK_LBUTTON))
			OnRelease();
	}
}

void ScriptCheckBoxFullScreen::Exit()
{

}

void ScriptCheckBoxFullScreen::SetOwner(std::shared_ptr<GOC> _gameObjPtr)
{
	gameObjPtr = _gameObjPtr;

	colliderPtr = _gameObjPtr->has(BoxCollider);
	transformPtr = _gameObjPtr->has(Transform);
	spritePtr = _gameObjPtr->has(SpriteComponent);
}

void ScriptCheckBoxFullScreen::OnRelease()
{
	if (WINDOWS->isWindowMode)
	{
		WINDOWS->FullScreenSwitch();
		GRAPHICS->UpdateScreenSize();
		spritePtr->SetCurrentFrame(5);
		CORE->SetFullScreen(true);
	}
	else
	{
		WINDOWS->FullScreenSwitch();
		GRAPHICS->UpdateScreenSize();
		spritePtr->SetCurrentFrame(4);
		CORE->SetFullScreen(false);
	}
}

void ScriptCheckBoxFullScreen::OnHold()
{
}

void ScriptCheckBoxFullScreen::LoadResource()
{
	FACTORY->CreateArcheType("FullScreenCheckBox");
}
