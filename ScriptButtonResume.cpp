/*************************************************************************************/
/*
File Name:		ScriptButtonResume.cpp
Project Name:	WhereGotLate
Author(s):		Kok Zhe Yan			 [100%]


Copyright Information:
All content © 2019 DigiPen (SINGAPORE) Corporation, all rights reserved.
*/
/*************************************************************************************/
#include "stdafx.h"
#include "ScriptButtonResume.h"

// to quit the game
#include "Logic.h"
#include "PauseMenuScript.h"


void ScriptButtonResume::Update(float _dt)
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

void ScriptButtonResume::OnRelease()
{
	LOGIC->GetScript<PauseMenuScript>("PAUSEMENU_SCRIPT")->DestroyMenu();
	if (selectedPtr.lock() != nullptr)
		selectedPtr.lock()->Destroy();
}
