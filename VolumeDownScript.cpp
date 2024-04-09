/*************************************************************************************/
/*
File Name:		VolumeDownScript.cpp
Project Name:	WhereGotLate
Author(s):		Kok Zhe Yan			 [100%]


Copyright Information:
All content © 2019 DigiPen (SINGAPORE) Corporation, all rights reserved.
*/
/*************************************************************************************/
#include "stdafx.h"
#include "VolumeDownScript.h"

#include "Logic.h"
#include "FModSoundManager.h"

VolumeDownScript::~VolumeDownScript()
{
}

void VolumeDownScript::Start()
{
}

void VolumeDownScript::Update(float _dt)
{
	UNREFERENCED_PARAMETER(_dt);

	if (SOUNDMGR->GetMasterVolume() == 0.0f)
	{
		spritePtr->SetCurrentFrame(spritePtr->GetStartFrame() + 2);
		return;
	}

	if (PHYSICS->IsCollidingWithMouse(colliderPtr))
	{
		//if (INPUTMGR->GetKeyPressed(MVK_LBUTTON))
			//OnClick();
		spritePtr->SetCurrentFrame(spritePtr->GetStartFrame() + 1);
		if (INPUTMGR->GetKeyHold(MVK_LBUTTON))
			OnHold();
		if (INPUTMGR->GetKeyRelease(MVK_LBUTTON))
			OnRelease();
	}
	else // mouse not selecting button
	{
		spritePtr->SetCurrentFrame(spritePtr->GetStartFrame());
	}
}

void VolumeDownScript::Exit()
{
}

void VolumeDownScript::SetOwner(std::shared_ptr<GOC> _gameObjPtr)
{
	gameObjPtr = _gameObjPtr;

	colliderPtr = _gameObjPtr->has(BoxCollider);
	transformPtr = _gameObjPtr->has(Transform);
	spritePtr = _gameObjPtr->has(SpriteComponent);
}

void VolumeDownScript::OnRelease()
{
	SOUNDMGR->DecrementMasterVolume(0.1f);
}

void VolumeDownScript::OnHold()
{
	spritePtr->SetCurrentFrame(spritePtr->GetStartFrame() + 1);
}

void VolumeDownScript::LoadResource()
{
	FACTORY->CreateArcheType("VolumeDown");
}
