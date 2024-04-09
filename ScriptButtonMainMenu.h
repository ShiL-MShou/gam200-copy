/*****************************************************************************/
/*
File Name:		ScriptButtonMainMenu.h
Project Name:	WhereGotLate
Author(s):		Tay Qin Wen Alvin     [100%]

Copyright information:
All content copyright 2020 DigiPen (SINGAPORE) Corporation, all rights reserved.
*/
/*****************************************************************************/
#pragma once
#include "ScriptButton.h"


class ScriptButtonMainMenu : public ScriptButton
{

public:
	void Update(float _dt) override;

	void OnRelease() override;
};

