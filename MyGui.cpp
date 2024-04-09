/*************************************************************************************/
/*
File Name:		MyGui.cpp
Project Name:	WhereGotLate
Author(s):		Kok Zhe Yan			 [100%]


Copyright Information:
All content © 2019 DigiPen (SINGAPORE) Corporation, all rights reserved.
*/
/*************************************************************************************/
#include "stdafx.h"

#include <filesystem>

#include "MyGui.h"
#include "MyGuiManager.h"

#include "CoreEngine.h"
#include "Logic.h"

void MyGui::LoadScene(const std::string& _scenename)
{
	LOGIC->ChangeLevel(_scenename);
	MyGuiManager::curr_obj_id = -1;
}

void MyGui::SaveScene(const std::string& _scenename)
{
	LOGIC->SaveLevel(_scenename);
	//LOGIC->SaveLevel("TestSave");
}

bool MyGui::IsFileExist(const std::string& _fileName)
{
	std::ifstream infile(_fileName);
	return infile.good();
}

int MyGui::MyResizeCallback(ImGuiInputTextCallbackData* data)
{
	if (data->EventFlag == ImGuiInputTextFlags_CallbackResize)
	{
		ImVector<char>* my_str = (ImVector<char>*)data->UserData;
		IM_ASSERT(my_str->begin() == data->Buf);
		my_str->resize(data->BufSize);
		data->Buf = my_str->begin();
	}
	return 0;
}

bool MyGui::MyInputText(const char* label, ImVector<char>* my_str, ImGuiInputTextFlags flags)
{
	IM_ASSERT((flags & ImGuiInputTextFlags_CallbackResize) == 0);
	return ImGui::InputText(label, my_str->begin(), (size_t)my_str->size(), flags | ImGuiInputTextFlags_CallbackResize, MyResizeCallback, (void*)my_str);
}
