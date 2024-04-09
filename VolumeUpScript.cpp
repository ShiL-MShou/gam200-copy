/*************************************************************************************/
/*
File Name:		VolumeUpScript.cpp
Project Name:	WhereGotLate
Author(s):		Kok Zhe Yan			 [100%]


Copyright Information:
All content © 2019 DigiPen (SINGAPORE) Corporation, all rights reserved.
*/
/*************************************************************************************/
#include "stdafx.h"
#include "VolumeUpScript.h"

#include "Logic.h"
#include "FModSoundManager.h"

VolumeUpScript::~VolumeUpScript()
{
}

void VolumeUpScript::Start()
{
}

void VolumeUpScript::Update(float _dt)
{
	UNREFERENCED_PARAMETER(_dt);

	if (SOUNDMGR->GetMasterVolume() == 1.0f)
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

void VolumeUpScript::Exit()
{
}

void VolumeUpScript::SetOwner(std::shared_ptr<GOC> _gameObjPtr)
{
	gameObjPtr = _gameObjPtr;

	colliderPtr = _gameObjPtr->has(BoxCollider);
	transformPtr = _gameObjPtr->has(Transform);
	spritePtr = _gameObjPtr->has(SpriteComponent);
}

void VolumeUpScript::OnRelease()
{
	SOUNDMGR->IncrementMasterVolume(0.1f);
}

void VolumeUpScript::OnHold()
{
	spritePtr->SetCurrentFrame(spritePtr->GetStartFrame() + 1);
}

void VolumeUpScript::LoadResource()
{
	FACTORY->CreateArcheType("VolumeUp");
}
