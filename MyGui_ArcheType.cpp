/*************************************************************************************/
/*
File Name:		MyGui_ArcheType.cpp
Project Name:	WhereGotLate
Author(s):		Kok Zhe Yan			 [100%]


Copyright Information:
All content © 2019 DigiPen (SINGAPORE) Corporation, all rights reserved.
*/
/*************************************************************************************/
#include "stdafx.h"

#include <filesystem>

#include "MyGui_ArcheType.h"
#include "MyGuiManager.h"

#include "Graphics.h"
#include "ComponentCreator.h"

static std::string currarchetype = "";
static const char* input = "";

//const std::string path = CORE->mPath + "Objects/";
const std::string extension = ".json";

namespace fs = std::filesystem;

MyGui_ArcheType::MyGui_ArcheType() : focus{ false }, path{ CORE->mPath + "Objects/" }
{
}

void MyGui_ArcheType::Open()
{
}

void MyGui_ArcheType::Update()
{
	if (MyGuiManager::show_archetype_editor) LoadArchetypeEditor(&MyGuiManager::show_archetype_editor);
}

void MyGui_ArcheType::Exit()
{
}

void MyGui_ArcheType::LoadArchetypeEditor(bool* _open)
{
	ImGui::SetNextWindowPos(ImVec2(0, 20), ImGuiCond_FirstUseEver);
	ImGui::SetNextWindowSize(ImVec2(550, 680), ImGuiCond_FirstUseEver);

	if (!ImGui::Begin("Archetype Editor", _open))
	{
		ImGui::End();
		return;
	}
	if (ImGui::Button("Create New Archetype"))
		ImGui::OpenPopup("Create New Archetype");
	if (strcmp(currarchetype.c_str(), "") && ImGui::Button("Save Archetype"))
	{
		FACTORY->SaveArchetype(currarchetype);
	}
	if (ImGui::Button("Remove Archetype"))
		ImGui::OpenPopup("Remove Archetype");

	bool open = true;
	if (ImGui::BeginPopupModal("Create New Archetype", &open, ImGuiWindowFlags_AlwaysAutoResize))
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
		input = objname.c_str();

		if (IsFileExist(path + input + extension))
		{
			ImGui::Text("ArcheType Already Exists");
		}
		else
		{
			if (!strcmp(input, ""))
				ImGui::Text("Please enter a new name.\n");
			else
			{
				ImGui::SameLine();
				if (ImGui::Button("Create"))
				{
					FACTORY->AddArcheType(input);
					FACTORY->SaveArchetype(input);
					objstr.clear();
					ImGui::CloseCurrentPopup();
				}
			}
		}
		ImGui::EndPopup();
	}

	open = true;
	if (ImGui::BeginPopupModal("Remove Archetype", &open, ImGuiWindowFlags_AlwaysAutoResize))
	{
		ImGui::BeginChild("Archetype list", ImVec2(200, 200), true);
		static std::string currfilename = "";
		for (auto& p : fs::directory_iterator(CORE->mPath + "Objects/"))
		{
			bool isSelected = (currfilename == p.path().stem().string().c_str());
			if (ImGui::Selectable(p.path().stem().string().c_str(), isSelected))
				currfilename = p.path().stem().string().c_str();
		}
		ImGui::EndChild();

		/*std::string path = CORE->mPath + "Level Files/";
		std::string extension = ".json";*/

		if (ImGui::Button("Remove"))
		{
			ImGui::OpenPopup("Confirm Remove");
		}

		if (ImGui::BeginPopupModal("Confirm Remove"))
		{
			ImGui::Text("Are you sure you want \n delete this Archetype?");
			if (ImGui::Button("Yes"))
			{
				fs::remove(path + currfilename + extension);
				ImGui::CloseCurrentPopup();
			}
			ImGui::SameLine();
			if (ImGui::Button("No"))
			{
				ImGui::CloseCurrentPopup();
			}
			ImGui::EndPopup();
		}
		ImGui::EndPopup();
	}

	ImGui::Separator();
	ImGui::Text("ArcheTypes");

	//static std::string currentobj = "";
	ImGui::BeginChild("objectlist", ImVec2(0, 150), true);

	for (auto& p : fs::directory_iterator(CORE->mPath + "Objects"))
	{
		bool isSelected = (currarchetype == p.path().stem().string().c_str());
		if (ImGui::Selectable(p.path().stem().string().c_str(), isSelected))
		{
			currarchetype = p.path().stem().string().c_str();
		}
	}

	ImGui::EndChild();

	auto archemap = FACTORY->GetArcheTypeMap();

	if (strcmp(currarchetype.c_str(), ""))
	{
		FACTORY->CreateArcheType(currarchetype);
		archemap = FACTORY->GetArcheTypeMap();
		std::shared_ptr<GOC> object = archemap[currarchetype];


		//ImGui::BeginChild("Sprite");

		if (std::shared_ptr<SpriteComponent> sprite = object->has(SpriteComponent))
		{
			std::shared_ptr<Transform> transform = object->has(Transform);
			if (sprite->GetType() == SpriteType::SINGLE)
			{
				std::shared_ptr<Texture> texture = GRAPHICS->GetTexture(sprite->GetCurrTextureName());
				//std::cout << sprite->GetCurrSubTextureName() << std::endl;

				std::shared_ptr<SpriteList> tmpSprList = GRAPHICS->GetSpriteList(sprite->GetCurrSpriteListName());

				Sprite& spr = tmpSprList->sprites.at(sprite->GetCurrFrameNum());

				VEC2 uv1{ spr.uv[1] };
				VEC2 uv3{ spr.uv[3] };

				//Maybe count the ratio of the width and height den scale the longer side to 100 den get the other side using the ratio?
				// setting the size

				ImGui::Image(reinterpret_cast<void*>(static_cast<intptr_t>(texture->GetRenderID())),
					ImVec2(100.0f, 100.0f),
					ImVec2(uv3.x, uv3.y),
					ImVec2(uv1.x, uv1.y));
			}
			else // animated
			{
				std::shared_ptr<AnimationComponent> animationComp = object->has(AnimationComponent);

				std::shared_ptr<Animation> animation = GRAPHICS->GetAnimation(animationComp->animationList.at(animationComp->currAnimation));

				std::shared_ptr<Texture> texture = GRAPHICS->GetTexture(animation->textureName);
				Frame& frame = animation->frames.at(animationComp->currFrame);
				
				//Maybe count the ratio of the width and height den scale the longer side to 100 den get the other side using the ratio?
				// setting the size

				ImGui::Image(reinterpret_cast<void*>(static_cast<intptr_t>(texture->GetRenderID())),
					ImVec2(100.0f, 100.0f),
					ImVec2(frame.uv[3].x, frame.uv[3].y),
					ImVec2(frame.uv[1].x, frame.uv[1].y));
			}

		}
		else
			ImGui::Text("Current Object has\nno Sprite");
		//ImGui::EndChild();



		auto componentmap = archemap[currarchetype]->GetComponentMap();

		for (auto [name, id] : componentmap)
		{
			if (ImGui::CollapsingHeader(name.c_str()))
			{
				archemap[currarchetype]->GetComponent(name)->Inspect();
			}
		}
		ImGui::Text("\n");
		if (ImGui::Button("Add Component"))
		{
			ImGui::OpenPopup("ComponentAddList");
		}
		if (ImGui::Button("Remove Component"))
		{
			ImGui::OpenPopup("ComponentRemoveList");
		}
	}
	// Add Component Popup
	open = true;
	if (ImGui::BeginPopupModal("ComponentAddList", &open, ImGuiWindowFlags_AlwaysAutoResize))
	{
		ImGui::BeginChild("componentlist", ImVec2(150, 100), true);
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
			archemap[currarchetype]->AddComponent(currentcomponent, newcomponent->GetID());
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
		auto componentmap = archemap[currarchetype]->GetComponentMap();
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
			archemap[currarchetype]->RemoveComponent(currentcomponent);
			ImGui::CloseCurrentPopup();
		}
		ImGui::EndPopup();
	}

	ImGui::End();
}

