/*****************************************************************************/
/*
File Name:		MainMenuScript.h
Project Name:	WhereGotLate
Author(s):		Ong Cheng Kai Marcus [100%]

Copyright information:
All content copyright 2020 DigiPen (SINGAPORE) Corporation, all rights reserved.
*/
/*****************************************************************************/
#pragma once

#include "Script.h"

class MainMenuScript : public Script
{
	bool playmusic;
public:
	void Start() override;

	void Update(float _dt) override;

	void Exit() override;
};
extern bool notInGame; //only true when we change state into game. 