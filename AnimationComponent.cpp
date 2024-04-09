/************************************************************************************ /
/*
File Name:		AnimationComponent.cpp
Project Name:	WhereGotLate
Author(s):		Tay Qin Wen Alvin [100%]


Brief:			Animation component.


Copyright Information:
All content © 2019 DigiPen (SINGAPORE) Corporation, all rights reserved.
*/
/*************************************************************************************/


#include "stdafx.h"
#include "AnimationComponent.h"

// to get the animation id
#include "Graphics.h"

#include "SpriteComponent.h"

// for inspect
#include <imgui_widgets.cpp>


AnimationComponent::AnimationComponent()
	: currAnimation{ 0 }, startingAnimation{ 0 }, currFrame{ 0 }, currFrameTime{ 0 }, repeat{ true }
{
}

AnimationComponent::~AnimationComponent()
{
}

void AnimationComponent::Initialize()
{
	GRAPHICS->AddAnimationComponent(GetBase()->has(AnimationComponent));
}

void AnimationComponent::Inspect()
{
	ImGui::Text("Animation List: ");

	// --- Animation List ---
	ImGui::BeginChild("MyAnimationList", ImVec2(150, 200), true);
	{
		static const char* currtexture = GRAPHICS->GetAnimationName(animationList.at(currAnimation)).c_str();

		for (int i = 0; i < animationList.size(); ++i)
		{
			std::string tmpAnimationName = GRAPHICS->GetAnimationName(animationList.at(i)).c_str();

			// selection
			bool isSelected = (currtexture == tmpAnimationName);
			if (ImGui::Selectable(tmpAnimationName.c_str(), isSelected))
			{
				// update selection
				GetBase()->has(SpriteComponent)->ChangeAnimation(tmpAnimationName);
			}
		}
	}
	ImGui::EndChild();


	if (ImGui::Button("Add"))
	{
		ImGui::OpenPopup("Animation List");
	}

	bool open = true;
	if (ImGui::BeginPopupModal("Animation List", &open, ImGuiWindowFlags_AlwaysAutoResize))
	{
		static std::string tmpAnimationName, currAnimationName = "";

		// --- texture selection ---
		ImGui::BeginChild("FullAnimationList", ImVec2(150, 50), true);
		{
			for (int i = 0; i < GRAPHICS->GetAnimationList().size(); ++i)
			{
				tmpAnimationName = GRAPHICS->GetAnimationList().at(i)->name;

				// selection
				bool isSelected = (strcmp(currAnimationName.c_str(), tmpAnimationName.c_str()) == 0);
				if (ImGui::Selectable(tmpAnimationName.c_str(), isSelected))
				{
					// update selection
					currAnimationName = tmpAnimationName;
				}
			}
		}
		ImGui::EndChild();

		if (ImGui::Button("Add"))
		{
			// add to list
			std::shared_ptr<AnimationComponent> animationCompPtr = GetBase()->has(AnimationComponent);
			
			int newIndex = GRAPHICS->GetAnimationID(currAnimationName);

			
			if (std::find(std::begin(animationCompPtr->animationList), std::end(animationCompPtr->animationList), newIndex) == std::end(animationCompPtr->animationList))
				animationCompPtr->animationList.push_back(newIndex);

			GetBase()->has(SpriteComponent)->ChangeAnimation(currAnimationName);
			ImGui::CloseCurrentPopup();
		}
		ImGui::EndPopup();
	}

	if (ImGui::Button("Set Start Animation"))
	{
		startingAnimation = currAnimation + 1;
	}
}

std::shared_ptr<Component> AnimationComponent::Clone()
{
	std::shared_ptr<AnimationComponent> newComponent = std::make_shared<AnimationComponent>();

	for (auto animationID : animationList)
	{
		newComponent->animationList.push_back(animationID);
	}

	newComponent->currAnimation = currAnimation;
	newComponent->startingAnimation = startingAnimation;
	newComponent->repeat = repeat;


	return newComponent;
}

void AnimationComponent::Destroy()
{
	MyAnimationManager.RemoveComponent(GetBase()->has(AnimationComponent));
	animationList.clear();
}

// not tested
void AnimationComponent::Serialize(JsonReader& _jr)
{
	std::string compName = "AnimationComponent";

	int animationCount = 0;
	std::string animationName = "";
	
	std::string toRepeat = "invalid";


	_jr.ReadIntMember(compName, "AnimationCount", animationCount);

	for (int i = 1; i <= animationCount; ++i)
	{
		_jr.ReadStringMember(compName, "AnimationName" + std::to_string(i), animationName);
		
		// add into the animation list
		animationList.push_back(GRAPHICS->GetAnimationID(animationName));
	}
	
	_jr.ReadIntMember(compName, "StartingAnimation", startingAnimation);

	_jr.ReadStringMember(compName, "Repeat", toRepeat);

	if (toRepeat != "invalid")
	{
		if (toRepeat == "FALSE")
			repeat = false;
	}
}

// not tested
void AnimationComponent::DeSerialize(JsonWriter& _jw)
{
	_jw.CreateKey(GetName());
	_jw.prettyWriterBuffer.StartObject();

	_jw.CreateIntMember("AnimationCount", static_cast<int>(animationList.size()));

	for (int i = 0; i < animationList.size(); ++i)
	{
		_jw.CreateStringMember("AnimationName" + std::to_string(i), GRAPHICS->GetAnimationName(animationList.at(i)));
	}

	_jw.CreateIntMember("StartingAnimation", startingAnimation);

	_jw.prettyWriterBuffer.EndObject();
}
