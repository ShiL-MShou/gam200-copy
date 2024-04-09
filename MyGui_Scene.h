/*************************************************************************************/
/*
File Name:		MyGui_Scene.h
Project Name:	WhereGotLate
Author(s):		Kok Zhe Yan			 [100%]


Copyright Information:
All content © 2019 DigiPen (SINGAPORE) Corporation, all rights reserved.
*/
/*************************************************************************************/
#pragma once

#include "MyGui.h"


class MyGui_Scene :	public MyGui
{
	bool show_scene_editor;
	bool focus;
	const char* curr_obj_name;

	void LoadSceneEditor(bool* _open);


public:

	MyGui_Scene() : show_scene_editor{ true }, focus{ false }, curr_obj_name{ "" }
	{}

	void Open() override;

	void Update() override;

	void Exit() override;
};

