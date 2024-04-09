/*************************************************************************************/
/*
File Name:		MyGui_Menubar.h
Project Name:	WhereGotLate
Author(s):		Kok Zhe Yan			 [100%]


Copyright Information:
All content © 2019 DigiPen (SINGAPORE) Corporation, all rights reserved.
*/
/*************************************************************************************/
#pragma once

#include "MyGui.h"

class MyGui_Menubar : public MyGui
{
	// for user input and file system management
	//enum class FileMode
	//{
	//	FM_None = 0,
	//	FM_NewFile,
	//	FM_RemoveFile
	//	//FM_ChangeName
	//};

	// status of text input window
	//bool show_text_input;
	// bool use to set focus of the keyboard cursor
	bool focus;
	// vector holding the text buffer of the input window
	//ImVector<char> mystr;
	// converts the vector of char to a string
	//const char* text_input;
	// file system mode
	//FileMode filemode;

	void LoadMainMenuBar();
	void LoadFileSystem();
	//void LoadTextInput(bool* _open);

	void NewFile();
	void RemoveFile();
	// void ChangeFileName();

public:
	MyGui_Menubar() : focus{ false }//show_text_input{ false }, text_input{ "" }//, filemode{ FileMode::FM_None }, focus{ false }
	{}

	//~MyGui_Menubar() { mystr.clear(); text_input = ""; }

	void Open() override;

	void Update() override;

	void Exit() override;
};

