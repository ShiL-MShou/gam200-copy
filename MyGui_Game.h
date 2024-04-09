/*************************************************************************************/
/*
File Name:		MyGui_Game.h
Project Name:	WhereGotLate
Author(s):		Kok Zhe Yan			 [100%]


Copyright Information:
All content © 2019 DigiPen (SINGAPORE) Corporation, all rights reserved.
*/
/*************************************************************************************/
#pragma once
#include "MyGui.h"


class MyGui_Game : public MyGui
{
	void LoadGameWindow(bool* _open);

public:
	
	void Open() override;

	void Update() override;

	void Exit() override;
};

