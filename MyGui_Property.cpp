/*************************************************************************************/
/*
File Name:		MyGui_Property.cpp
Project Name:	WhereGotLate
Author(s):		Kok Zhe Yan			 [100%]


Copyright Information:
All content © 2019 DigiPen (SINGAPORE) Corporation, all rights reserved.
*/
/*************************************************************************************/
#include "stdafx.h"

#include "MyGui_Property.h"
#include "MyGuiManager.h"

#include "ComponentCreator.h"
#include "DebugDraw.h"

void MyGui_Property::Open()
{
}

void MyGui_Property::Update()
{
	if (MyGuiManager::show_property_editor)	LoadPropertyEditor(&MyGuiManager::show_property_editor);
}

void MyGui_Property::Exit()
{
}

void MyGui_Property::LoadPropertyEditor(bool* _open)
{
	// setting up of the window
	//ImGuiWindowFlags window_flags = 0;
	//window_flags |= ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse;
	//_open = NULL;

	ImGui::SetNextWindowPos(ImVec2(0, 20), ImGuiCond_FirstUseEver);
	ImGui::SetNextWindowSize(ImVec2(550, 680), ImGuiCond_FirstUseEver);

	if (!ImGui::Begin("Property Editor", _open))
	{
		ImGui::End();
		return;
	}

	if (MyGuiManager::curr_obj_id == -1)
	{
		MyDebugDraw.DrawSelected(VEC2{}, VEC2{});
		ImGui::End();
		return;
	}
	
	std::shared_ptr<GOC> currobj = FACTORY->GetObjectWithID(MyGuiManager::curr_obj_id);
	//auto objmap = FACTORY->GetObjMap();

	//ImGui::TextColored(ImVec4(0.2f, 0.2f, 0.8f, 0.8f), currobj->GetName());
	ImGui::Text(currobj->GetName());
	ImGui::Text(currobj->GetType());
	ImGui::Separator();

	if (ImGui::Button("Add Component"))
	{
		ImGui::OpenPopup("ComponentAddList");
	}
	//ImGui::PushStyleColor(ImGuiCol_Button, (ImVec4)ImColor::HSV(0.0f, 1.0f, 1.0f, 0.5f));
	if (ImGui::Button("Remove Component"))
	{
		ImGui::OpenPopup("ComponentRemoveList");
	}
	//ImGui::PopStyleColor(1);

	// Add Component Popup
	bool open = true;
	if (ImGui::BeginPopupModal("ComponentAddList", &open, ImGuiWindowFlags_AlwaysAutoResize))
	{
		ImGui::BeginChild("Componentlist", ImVec2(150, 100), true);
		static std::string currentcomponent = "";
		auto creatormap = FACTORY->GetCreatorsMap();
		for (auto [cname, creatorptr] : creatormap)
		{
			bool isSelected = (currentcomponent == cname);
			if (ImGui::Selectable(cname.c_str(), isSelected))
			{
				currentcomponent = cname.c_str();
			}
		}
		ImGui::EndChild();
		if (ImGui::Button("Add"))
		{
			std::shared_ptr<Component> newcomponent = FACTORY->ConstructComponent(currentcomponent);
			currobj->AddComponent(currentcomponent, newcomponent->GetID());
			newcomponent->SetBase(currobj);
			newcomponent->Initialize();
			ImGui::CloseCurrentPopup();
		}
		ImGui::EndPopup();
	}
	// Remove Component Popup
	open = true;
	if (ImGui::BeginPopupModal("ComponentRemoveList", &open, ImGuiWindowFlags_AlwaysAutoResize))
	{
		ImGui::BeginChild("Componentlist", ImVec2(150, 100), true);
		static std::string currentcomponent = "";
		auto componentmap = currobj->GetComponentMap();
		for (auto [name, id] : componentmap)
		{
			bool isSelected = (currentcomponent == name);
			if (ImGui::Selectable(name.c_str(), isSelected))
			{
				currentcomponent = name;
			}
		}
		ImGui::EndChild();

		if (ImGui::Button("Remove"))
		{
			currobj->RemoveComponent(currentcomponent);
			ImGui::CloseCurrentPopup();
		}
		ImGui::EndPopup();
	}
	
	
	MyDebugDraw.DrawSelected(currobj->has(Transform)->GetPosition(), currobj->has(Transform)->GetScale());

	auto componentmap = currobj->GetComponentMap();

	for (auto [name, id] : componentmap)
	{
		ImGui::PushStyleColor(ImGuiCol_Button, (ImVec4)ImColor::HSV(2 / 7.0f, 0.6f, 0.6f));
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, (ImVec4)ImColor::HSV(2 / 7.0f, 0.7f, 0.7f));
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, (ImVec4)ImColor::HSV(2 / 7.0f, 0.8f, 0.8f));
		if (ImGui::CollapsingHeader(name.c_str()))
		{
			currobj->GetComponent(name)->Inspect();
		}
		ImGui::PopStyleColor(3);
	}
	
	ImGui::End();
}

