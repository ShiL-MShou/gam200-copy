/*************************************************************************************/
/*
File Name:		MyGui_Menubar.cpp
Project Name:	WhereGotLate
Author(s):		Kok Zhe Yan			 [100%]


Copyright Information:
All content © 2019 DigiPen (SINGAPORE) Corporation, all rights reserved.
*/
/*************************************************************************************/
#include "stdafx.h"

#include <filesystem>

#include "MyGui_Menubar.h"
#include "MyGuiManager.h"

#include "CoreEngine.h"
#include "Logic.h"

namespace fs = std::filesystem;


void MyGui_Menubar::Open()
{
}

void MyGui_Menubar::Update()
{
	LoadMainMenuBar();
	//if (MyGuiManager::show_text_input)	LoadTextInput(&MyGuiManager::show_text_input);
}

void MyGui_Menubar::Exit()
{
}

void MyGui_Menubar::LoadMainMenuBar()
{
	if (ImGui::BeginMainMenuBar())
	{
		if (ImGui::BeginMenu("File"))
		{
			LoadFileSystem();
			ImGui::EndMenu();
		}
		if (ImGui::BeginMenu("Windows"))
		{
			ImGui::Checkbox("Archetype Editor", &MyGuiManager::show_archetype_editor);
			ImGui::Checkbox("Performance Data", &MyGuiManager::show_performance_data);
			ImGui::Checkbox("Property Editor", &MyGuiManager::show_property_editor);
			ImGui::Checkbox("Scene Editor", &MyGuiManager::show_scene_editor);
			ImGui::Checkbox("Game Window", &MyGuiManager::show_game_window);
			ImGui::Checkbox("Gizmo", &MyGuiManager::show_gizmo);
			ImGui::Checkbox("Tile Editor", &MyGuiManager::show_tile_editor);
			ImGui::EndMenu();
		}
		if (ImGui::BeginMenu("Test"))
		{
			//if (ImGui::Button("Win"))
			//{
			//	LOGIC->CreateObjectAt(VEC2(640, 500), 0, "Win", "Win", 0);
			//	LOGIC->CreateObjectAt(VEC2(640, 300), 0, "RestartButton", "RestartBtn", 0);
			//	//LOGIC->CreateObjectAt(VEC2(640, 360), 0, "Tint", "Tint", 0);
			//	LOGIC->CreateObjectAt(VEC2(640, 170), 0, "TitleButton", "TitleBtn", 0);
			//}
			//if (ImGui::Button("Lose"))
			//{
			//	LOGIC->CreateObjectAt(VEC2(640, 500), 0, "Lose", "Lose", 0);
			//	LOGIC->CreateObjectAt(VEC2(640, 300), 0, "NextLevelButton", "NextLevelBtn", 0);
			//	//LOGIC->CreateObjectAt(VEC2(640, 360), 0, "Tint", "Tint", 0);
			//	LOGIC->CreateObjectAt(VEC2(640, 170), 0, "TitleButton", "TitleBtn", 0);
			//}
			ImGui::Separator();
			ImGui::Text("Number of Objects:");
			ImGui::SameLine();
			ImGui::Text(std::to_string(MyResource.GetNumObj()).c_str());
			ImGui::EndMenu();
		}
		ImGui::SameLine(1100.0f);
		//ImGui::Checkbox("Pause", &MyGuiManager::isGamePaused);
		ImGui::Text(" %.1f FPS (%.3f ms)", ImGui::GetIO().Framerate, 1000.0f / ImGui::GetIO().Framerate);
		ImGui::EndMainMenuBar();
	}
}

void MyGui_Menubar::LoadFileSystem()
{
	ImGui::MenuItem(CORE->GetCurrLevel().c_str(), NULL, false, false);
	ImGui::Separator();
	if (ImGui::BeginMenu("New Scene"))//, NULL, &show_text_input))
	{
		focus = true;
		NewFile();

		ImGui::EndMenu();
	}
	if (ImGui::BeginMenu("Load Scene"))
	{
		for (auto& p : fs::directory_iterator(CORE->mPath + "Level Files"))
		{
			if (ImGui::MenuItem(p.path().stem().string().c_str()))
			{
				LoadScene(p.path().stem().string().c_str());
			}
		}
		ImGui::EndMenu();
	}
	if (ImGui::MenuItem("Save Scene"))
	{
		SaveScene();
	}
	if (ImGui::BeginMenu("Remove Scene"))
	{
		//input_window_name = "Remove Scene";
		//filemode = FileMode::FM_RemoveFile;
		//focus = true;
		RemoveFile();
		ImGui::EndMenu();
	}
	if (ImGui::MenuItem("Exit"))
	{
		MessageQuit message;
		CORE->BroadcastMessage(message);
	}
	//if (ImGui::MenuItem("Change Scene Name", NULL, &show_text_input))
	//{
	//	input_window_name = "Change Scene Name";
	//	filemode = FileMode::FM_ChangeName;
	//}
}

//void MyGui_Menubar::LoadTextInput(bool* _open)
//{
//
//	ImGui::SetNextWindowPos(ImVec2(640, 380), ImGuiCond_FirstUseEver);
//	ImGui::SetNextWindowSize(ImVec2(200, 100), ImGuiCond_FirstUseEver);
//
//	if (!ImGui::Begin("New Scene", _open))
//	{
//		ImGui::End();
//		return;
//	}
//
//	//if (filemode == FileMode::FM_ChangeName)
//	//	ImGui::Text("Change the current filename");
//
//	if (mystr.empty())
//		mystr.push_back(0);
//
//	if (focus)	ImGui::SetKeyboardFocusHere();
//	MyInputText("##MyStr", &mystr);
//	focus = false;
//	std::string levelname;
//	for (auto& i : mystr)
//	{
//		levelname += i;
//	}
//	text_input = levelname.c_str();
//
//
//	switch (filemode)
//	{
//	case FileMode::FM_NewFile:
//		NewFile();
//		break;
//		//case FileMode::FM_ChangeName:
//		//	ChangeFileName();
//		//	break;
//	case FileMode::FM_RemoveFile:
//		RemoveFile();
//		break;
//	case FileMode::FM_None:
//		break;
//	}
//
//	ImGui::End();
//}

void MyGui_Menubar::NewFile()
{
	static ImVector<char> mystr;

	if (mystr.empty())
		mystr.push_back(0);

	//if (focus)	ImGui::SetKeyboardFocusHere();
	MyInputText("##MyStr", &mystr);
	//focus = false;
	std::string levelname;
	for (auto& i : mystr)
	{
		levelname += i;
	}
	static const char* text_input = levelname.c_str();

	std::string path = CORE->mPath + "Level Files/";
	std::string extension = ".json";
	if (IsFileExist(path + text_input + extension))
	{
		ImGui::Text("File Already Exists");
	}
	else
	{
		if (!strcmp(text_input, ""))
			ImGui::Text("Please enter a new filename.");
		else
		{
			ImGui::SameLine();
			if (ImGui::Button("Done"))
			{
				JsonWriter jw;

				jw.prettyWriterBuffer.StartObject();

				jw.CreateKey("Object1");
				jw.prettyWriterBuffer.StartObject();
				jw.CreateStringMember("ObjectName", "Camera");
				jw.CreateStringMember("ArcheType", "Camera");
				jw.CreateFloatMember("PosX", 0.0f);
				jw.CreateFloatMember("PosY", 0.0f);
				jw.CreateFloatMember("Rotation", 0.0f);
				jw.prettyWriterBuffer.EndObject();

				jw.prettyWriterBuffer.EndObject();

				jw.WriteToJsonFile(path + text_input + extension);

				LoadScene(text_input);
				mystr.clear();
			}
		}
	}
}

void MyGui_Menubar::RemoveFile()
{
	ImGui::BeginChild("Filelist", ImVec2(150,100), true);
	static std::string currfilename = "";
	for (auto& p : fs::directory_iterator(CORE->mPath + "Level Files/"))
	{
		bool isSelected = (currfilename == p.path().stem().string().c_str());
		if (ImGui::Selectable(p.path().stem().string().c_str(), isSelected))
			currfilename = p.path().stem().string().c_str();
	}
	ImGui::EndChild();

	std::string path = CORE->mPath + "Level Files/";
	std::string extension = ".json";

	if (currfilename == CORE->GetCurrLevel())
		ImGui::Text("This Scene is currently loaded,\nplease select another level.");
	else
	{
		if (ImGui::Button("Remove"))
		{
			fs::remove(path + currfilename + extension);
		}
	}
}

//void MyGui::ChangeFileName()
//{
//	std::string path = "Assets\\Level Files\\";
//	std::string extension = ".txt";
//
//	if (IsFileExist(path + text_input + extension))
//	{
//		ImGui::Text("FileName Already Exists");
//	}
//	else
//	{
//		if (ImGui::Button("Done"))
//		{
//			/*fs::directory_entry file{path + CORE->currLevel + extension };
//			file.replace_filename(path + text_input + extension);*/
//			//std::cout << fs::path(path + CORE->currLevel + extension).replace_filename(path + text_input + extension);
//			//fs::directory_entry(path + CORE->currLevel + extension).replace_filename(path + text_input + extension);
//			text_input = "";
//			my_str.clear();
//			show_text_input = false;
//		}
//	}
//}
