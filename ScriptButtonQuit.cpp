/************************************************************************************ /
/*
File Name:		ScriptButtonQuit.cpp
Project Name:	WhereGotLate
Author(s):		Tay Qin Wen Alvin [100%]


Brief:			Handle interaction for quit button.


Copyright Information:
All content © 2019 DigiPen (SINGAPORE) Corporation, all rights reserved.
*/
/*************************************************************************************/


#include "stdafx.h"
#include "ScriptButtonQuit.h"

// to quit the game
#include "Logic.h"

void ScriptButtonQuit::Update(float _dt)
{
	if (MyEvent.inConfirmQuit)
	{
		if (selectedPtr.lock() != nullptr)
			selectedPtr.lock()->Destroy();
		return;
	}
	else
		ScriptButton::Update(_dt);
}

void ScriptButtonQuit::OnRelease()
{
	std::shared_ptr<GOC> obj = LOGIC->CreateObjectAt(VEC2{}, 0.0f, "ConfirmQuit", "ConfirmQuit");
	obj->has(SpriteComponent)->SetFixedPosition(VEC2{ 650.0f, 350.0f });
}
