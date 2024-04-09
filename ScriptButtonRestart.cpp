/************************************************************************************ /
/*
File Name:		ScriptButtonRestart.cpp
Project Name:	WhereGotLate
Author(s):		Tay Qin Wen Alvin [100%]


Brief:			Handle interaction for restart button.


Copyright Information:
All content © 2019 DigiPen (SINGAPORE) Corporation, all rights reserved.
*/
/*************************************************************************************/

#include "stdafx.h"
#include "ScriptButtonRestart.h"

// to change levels
#include "Logic.h"


void ScriptButtonRestart::OnRelease()
{
	LOGIC->ChangeLevel();
}
