/*************************************************************************************/
/*
File Name:		MyGui_Property.h
Project Name:	WhereGotLate
Author(s):		Kok Zhe Yan			 [100%]


Copyright Information:
All content © 2019 DigiPen (SINGAPORE) Corporation, all rights reserved.
*/
/*************************************************************************************/
#pragma once

#include "MyGui.h"

class MyGui_Property : public MyGui
{
	bool show_property_editor;

	void LoadPropertyEditor(bool* _open);

public:
	MyGui_Property() : show_property_editor{ true }
	{}

	void Open() override;

	void Update() override;

	void Exit() override;

};

