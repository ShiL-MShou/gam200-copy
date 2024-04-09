/*************************************************************************************/
/*
File Name:		MyGui_Scene.cpp
Project Name:	WhereGotLate
Author(s):		Kok Zhe Yan			 [100%]


Copyright Information:
All content © 2019 DigiPen (SINGAPORE) Corporation, all rights reserved.
*/
/*************************************************************************************/
#include "stdafx.h"

#include <filesystem>

#include "MyGui_Scene.h"
#include "MyGuiManager.h"

#include "Logic.h"

#include "DebugDraw.h"

static std::string currarchetype = "";
namespace fs = std::filesystem;

void MyGui_Scene::Open()
{
}

void MyGui_Scene::Update()
{
	if (MyGuiManager::show_scene_editor)	LoadSceneEditor(&MyGuiManager::show_scene_editor);
}

void MyGui_Scene::Exit()
{
}

void MyGui_Scene::LoadSceneEditor(bool* _open)
{
	// setting up of the window
	//ImGuiWindowFlags window_flags = 0;
	//window_flags |= ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse;
	//_open = NULL;

	ImGui::SetNextWindowPos(ImVec2(0, 20), ImGuiCond_FirstUseEver);
	ImGui::SetNextWindowSize(ImVec2(550, 680), ImGuiCond_FirstUseEver);


	if (!ImGui::Begin("Scene Editor", _open))
	{
		ImGui::End();
			return;
	}
	if (ImGui::Button("Add Object"))
	{
		ImGui::OpenPopup("List of Archetype");
	}

	if (MyGuiManager::curr_obj_id != -1 && ImGui::Button("Remove Object"))
	{
		FACTORY->GetObjectWithID(MyGuiManager::curr_obj_id)->Destroy();
		MyGuiManager::curr_obj_id = -1;
	}
	ImGui::Separator();
	ImGui::Text("Objects");

	ImGui::BeginChild("Objects", ImVec2(0, 0), true);

	// Displays all the current objects in the scene
	auto objmap = MyResource.GetObjMap();
	for (auto& obj : objmap)
	{
		if (obj == nullptr)
			continue;
		
		bool isSelected = (curr_obj_name == obj->GetName());
		if (ImGui::Selectable(obj->GetName(), isSelected))
		{
			curr_obj_name = obj->GetName();
			MyGuiManager::curr_obj_id = obj->GetId();
		}
		
	}
	ImGui::EndChild();

	
	bool open = true;
	if (ImGui::BeginPopupModal("List of Archetype", &open, ImGuiWindowFlags_AlwaysAutoResize))
	{
		ImGui::BeginChild("objectlist", ImVec2(200, 400), true);

		//auto archemap = FACTORY->GetArcheTypeMap();
		for (auto& p : fs::directory_iterator(CORE->mPath + "Objects"))
		{
			bool isSelected = (currarchetype == p.path().stem().string().c_str());
			if (ImGui::Selectable(p.path().stem().string().c_str(), isSelected))
			{
				currarchetype = p.path().stem().string().c_str();
			}
		}

		ImGui::EndChild();

		if (strcmp(currarchetype.c_str(), ""))
		{
			//ImGui::Text("\n");
			if (ImGui::Button("Add"))
			{
				ImGui::OpenPopup("New Object Name");
				focus = true;
			}
			ImGui::SameLine();
			if (ImGui::Button("Done"))
			{
				ImGui::CloseCurrentPopup();
			}
		}
		// Prompt user for name of new object created
		if (ImGui::BeginPopupModal("New Object Name", &open, ImGuiWindowFlags_AlwaysAutoResize))
		{
			static ImVector<char> objstr;

			if (objstr.empty())
				objstr.push_back(0);
			if (focus)	ImGui::SetKeyboardFocusHere();
			MyInputText("##ObjStr", &objstr);
			focus = false;
			std::string objname;
			for (auto& i : objstr)
			{
				objname += i;
			}
			static const char* input = objname.c_str();

			auto objectmap = MyResource.GetObjMap();
			bool isobjexists = false;
			for (auto& obj : objectmap)
			{
				if (obj == nullptr)
					continue;
				if (strcmp(input, obj->GetName()) == 0)
					isobjexists = true;
			}
			if (isobjexists == true)
				ImGui::Text("Object Already Exists");
			else
			{
				if (!strcmp(input, ""))
					ImGui::Text("Enter a name for new object");
				else
				{
					if (ImGui::Button("Confirm"))
					{
						FACTORY->CreateArcheType(currarchetype);

						std::shared_ptr<Camera> camera = GRAPHICS->GetMainCameraPtr();
						VEC2 cameraoffset{ camera->GetScale().x * WinWidth * 0.5f, camera->GetScale().y * WinHeight * 0.5f };
						VEC2 objpos{ -camera->GetPosition().x + cameraoffset.x, -camera->GetPosition().y + cameraoffset.y };
						std::shared_ptr<GOC> newobj = LOGIC->CreateObjectAt(objpos, 0, currarchetype, input);
						MyGuiManager::curr_obj_id = newobj->GetId();
						objstr.clear();
						ImGui::CloseCurrentPopup();
					}
				}
			}
			ImGui::EndPopup();
		}

		ImGui::EndPopup();
	}

	ImGui::End();
}

