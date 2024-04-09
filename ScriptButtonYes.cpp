/*************************************************************************************/
/*
File Name:		ScriptButtonYes.cpp
Project Name:	WhereGotLate
Author(s):		Kok Zhe Yan			 [100%]


Copyright Information:
All content © 2019 DigiPen (SINGAPORE) Corporation, all rights reserved.
*/
/*************************************************************************************/
#include "stdafx.h"
#include "ScriptButtonYes.h"

#include "CoreEngine.h"


void ScriptButtonYes::OnRelease()
{
	MessageQuit message;
	CORE->BroadcastMessage(message);
}

void ScriptButtonYes::OnHold()
{
}

void ScriptButtonYes::CollisionResponse()
{
}
