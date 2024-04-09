/*************************************************************************************/
/*
File Name:		MouseCursorScript.cpp
Project Name:	WhereGotLate
Author(s):		Kok Zhe Yan			 [100%]


Copyright Information:
All content © 2019 DigiPen (SINGAPORE) Corporation, all rights reserved.
*/
/*************************************************************************************/
#include "stdafx.h"

#include "MouseCursorScript.h"

#include "WindowsSystem.h"
#include "Input.h"
#include "Graphics.h"

void MouseCursorScript::Update(float _dt)
{
	UNREFERENCED_PARAMETER(_dt);

	VEC2 mousepos = INPUTMGR->GetMousePosition();

	VEC2 scale = gameObjPtr.lock()->has(Transform)->GetScale();
	std::shared_ptr<SpriteComponent> sprite = gameObjPtr.lock()->has(SpriteComponent);
	sprite->SetCurrFrame(1);
	sprite->SetFixedPosition(mousepos);


	if (MyEvent.inGameStage)
	{
		sprite->SetActive(false);
	}
	else
		sprite->SetActive(true);

}

void MouseCursorScript::Start()
{
}

void MouseCursorScript::Exit()
{
}

void MouseCursorScript::SetOwner(std::shared_ptr<GOC> _gameObjPtr)
{
	gameObjPtr = _gameObjPtr;
}
