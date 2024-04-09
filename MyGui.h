/*************************************************************************************/
/*
File Name:		MyGui.h
Project Name:	WhereGotLate
Author(s):		Kok Zhe Yan			 [100%]


Copyright Information:
All content © 2019 DigiPen (SINGAPORE) Corporation, all rights reserved.
*/
/*************************************************************************************/
#pragma once

#include "imgui.h"
#include "Engine.h"
#include "CoreEngine.h"

class MyGui
{
public:
	virtual void Open() = 0;

	virtual void Update() = 0;

	virtual void Exit() = 0;

	static void LoadScene(const std::string& _scenename);
	static void SaveScene(const std::string& _scenename = CORE->GetCurrLevel());
	static bool IsFileExist(const std::string& _fileName);
	static int MyResizeCallback(ImGuiInputTextCallbackData* data);
	static bool MyInputText(const char* label, ImVector<char>* my_str, ImGuiInputTextFlags flags = 0);

	virtual ~MyGui() {}
};