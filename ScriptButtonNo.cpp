/*************************************************************************************/
/*
File Name:		ScriptButtonNo.cpp
Project Name:	WhereGotLate
Author(s):		Kok Zhe Yan			 [100%]


Copyright Information:
All content © 2019 DigiPen (SINGAPORE) Corporation, all rights reserved.
*/
/*************************************************************************************/
#include "stdafx.h"
#include "ScriptButtonNo.h"

// to change level
#include "Logic.h"
#include "ConfirmQuitScript.h"


void ScriptButtonNo::OnRelease()
{
	LOGIC->GetScript<ConfirmQuitScript>("CONFIRMQUIT_SCRIPT")->DestroyMenu();
	if (selectedPtr.lock() != nullptr)
		selectedPtr.lock()->Destroy();
}

void ScriptButtonNo::OnHold()
{
}

void ScriptButtonNo::CollisionResponse()
{
}
