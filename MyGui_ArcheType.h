/*************************************************************************************/
/*
File Name:		MyGui_ArcheType.h
Project Name:	WhereGotLate
Author(s):		Kok Zhe Yan			 [100%]


Copyright Information:
All content © 2019 DigiPen (SINGAPORE) Corporation, all rights reserved.
*/
/*************************************************************************************/
#pragma once
#include "MyGui.h"

class MyGui_ArcheType :	public MyGui
{
	bool focus;

	const std::string path;

	void LoadArchetypeEditor(bool* _open);

public:
	MyGui_ArcheType();

	void Open() override;

	void Update() override;

	void Exit() override;
};

