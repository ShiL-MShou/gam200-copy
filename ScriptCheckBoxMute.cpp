/*************************************************************************************/
/*
File Name:		ScriptCheckBoxMute.cpp
Project Name:	WhereGotLate
Author(s):		Kok Zhe Yan			 [100%]


Copyright Information:
All content © 2019 DigiPen (SINGAPORE) Corporation, all rights reserved.
*/
/*************************************************************************************/
#include "stdafx.h"
#include "ScriptCheckBoxMute.h"


#include "Logic.h"
//#include "Graphics.h"
#include "FModSoundManager.h"

ScriptCheckBoxMute::~ScriptCheckBoxMute()
{
}

void ScriptCheckBoxMute::Start()
{
}

void ScriptCheckBoxMute::Update(float _dt)
{
	UNREFERENCED_PARAMETER(_dt);

	if (PHYSICS->IsCollidingWithMouse(colliderPtr))
	{
		if (INPUTMGR->GetKeyHold(MVK_LBUTTON))
			OnHold();
		else if (INPUTMGR->GetKeyRelease(MVK_LBUTTON))
			OnRelease();
	}
}

void ScriptCheckBoxMute::Exit()
{
	isMute = false;
}

void ScriptCheckBoxMute::SetOwner(std::shared_ptr<GOC> _gameObjPtr)
{
	gameObjPtr = _gameObjPtr;

	colliderPtr = _gameObjPtr->has(BoxCollider);
	transformPtr = _gameObjPtr->has(Transform);
	spritePtr = _gameObjPtr->has(SpriteComponent);
}

void ScriptCheckBoxMute::OnRelease()
{
	if (isMute)
	{
		isMute = false;
		SOUNDMGR->MuteMasterVolume(false);
		spritePtr->SetCurrentFrame(spritePtr->GetStartFrame());
	}
	else
	{
		isMute = true;
		SOUNDMGR->MuteMasterVolume();
		spritePtr->SetCurrentFrame(spritePtr->GetStartFrame() + 1);
	}
}

void ScriptCheckBoxMute::OnHold()
{
}

void ScriptCheckBoxMute::LoadResource()
{
	FACTORY->CreateArcheType("MuteCheckBox");
}
