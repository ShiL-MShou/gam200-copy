/************************************************************************************ /
/*
File Name:		ScriptButton.cpp
Project Name:	WhereGotLate
Author(s):		Tay Qin Wen Alvin [100%]


Brief:			This is the base of all the script button.


Copyright Information:
All content © 2019 DigiPen (SINGAPORE) Corporation, all rights reserved.
*/
/*************************************************************************************/

#include "stdafx.h"
#include "ScriptButton.h"

#include "Logic.h"
#include "Graphics.h"
#include "Factory.h"

#include"WindowsSystem.h"

//sound and input
#include "FModSoundManager.h"
#include "Input.h"


ScriptButton::ScriptButton()
{
}

ScriptButton::~ScriptButton()
{
	Exit();
}

void ScriptButton::Start()
{
}

void ScriptButton::Update(float _dt)
{
	UNREFERENCED_PARAMETER(_dt);

	if (INPUTMGR->GetKeyPressed(MVK_LBUTTON))
		SOUNDMGR->PlaySoundFromMap("click");
	// check if the button is colliding with the mouse
	if (spritePtr->IsActive() && PHYSICS->IsCollidingWithMouse(colliderPtr))
	{
		//std::cout << "colliding" << std::endl;
		//if (INPUTMGR->GetKeyPressed(MVK_LBUTTON))
			//OnClick();

		if (selectedPtr.lock() == nullptr)
		{
			selectedPtr = LOGIC->CreateObjectAt(VEC2(transformPtr->GetPosition().x, transformPtr->GetPosition().y), 0.0f, "SelectedCircle", "SelectedCircle");
			selectedPtr.lock()->has(SpriteComponent)->SetFixedPosition(spritePtr->GetFixedPosition());
			selectedPtr.lock()->has(AnimationComponent)->repeat = false;
		}

		if (INPUTMGR->GetKeyHold(MVK_LBUTTON))
			OnHold();
		else if (INPUTMGR->GetKeyRelease(MVK_LBUTTON))
			OnRelease();

	}
	// reset the current frame when the mouse is not on the button
	else 
	{
		spritePtr->SetCurrentFrame(spritePtr->GetStartFrame());

		if (selectedPtr.lock() != nullptr)
			selectedPtr.lock()->Destroy();
	}


}

void ScriptButton::Exit()
{
	if (selectedPtr.lock() != nullptr)
		selectedPtr.lock()->Destroy();
}

void ScriptButton::SetOwner(std::shared_ptr<GOC> _gameObjPtr)
{
	gameObjPtr =	_gameObjPtr;

	colliderPtr =	_gameObjPtr->has(BoxCollider);
	transformPtr =	_gameObjPtr->has(Transform);
	spritePtr =		_gameObjPtr->has(SpriteComponent);
}

void ScriptButton::CollisionResponse()
{
}

void ScriptButton::OnHold()
{
	// udpating the current frame for feedback
	spritePtr->SetCurrentFrame(spritePtr->GetStartFrame() + 1);
}

void ScriptButton::LoadResource()
{
	// loading resources
	FACTORY->CreateArcheType("SelectedCircle");

	FACTORY->CreateArcheType("Transition");
}

void ScriptButton::OnRelease()
{
}

