/************************************************************************************ /
/*
File Name:		LogicComponent.cpp
Project Name:	WhereGotLate
Author(s):		Tay Qin Wen Alvin [100%]


Brief:			Logic component.


Copyright Information:
All content © 2019 DigiPen (SINGAPORE) Corporation, all rights reserved.
*/
/*************************************************************************************/

#include "stdafx.h"
#include "LogicComponent.h"

#include "Logic.h"

#include "imgui.h"

LogicComponent::LogicComponent()
	: isCollided{ false }, smName{"no"}
{
	scriptName = "PLAYER_SCRIPT";
}

LogicComponent::~LogicComponent()
{
}

void LogicComponent::Initialize()
{
	LOGIC->AddLogicComponent(GetID());
	LOGIC->GetScript(scriptName)->SetOwner(GetBase());
	LOGIC->GetScript(scriptName)->Init();
}

void LogicComponent::Inspect()
{
	ImGui::Text(smName.c_str());
	ImGui::Text(scriptName.c_str());
}



std::shared_ptr<Component> LogicComponent::Clone()
{
	std::shared_ptr<LogicComponent> newComponent = std::make_shared<LogicComponent>();

	newComponent->SetName(GetName());
	newComponent->smName = smName;
	newComponent->scriptName = scriptName;

	return newComponent;
}

void LogicComponent::Serialize(ISerialization& stream)
{
	UNREFERENCED_PARAMETER(stream);
}

void LogicComponent::Serialize(JsonReader& _jr)
{
	std::string name = "LogicComponent";
//	std::string button = "";


	_jr.ReadStringMember(name, "Script", scriptName);
	_jr.ReadStringMember(name, "smName", smName);
	//_jr.ReadStringMember(name, "ButtonType", button);

	//if (strcmp(button.c_str(), "MAINMENU") == 0)
	//	buttontype = BUTTONTYPE::MAINMENU;
	//else if (strcmp(button.c_str(), "PAUSEMENU") == 0)
	//	buttontype = BUTTONTYPE::PAUSEMENU;
	//else if (strcmp(button.c_str(), "QUITMENU") == 0)
	//	buttontype = BUTTONTYPE::QUITMENU;
	//else if (strcmp(button.c_str(), "TRANSMENU") == 0)
	//	buttontype = BUTTONTYPE::TRANSMENU;
	//else if (strcmp(button.c_str(), "") == 0)
	//	buttontype = BUTTONTYPE::NONE;

}

void LogicComponent::DeSerialize(JsonWriter& _jw)
{
	/*std::string button;

	if (buttontype == BUTTONTYPE::MAINMENU)
		button = "MAINMENU";
	else if	(buttontype == BUTTONTYPE::PAUSEMENU)
		button = "PAUSEMENU";
	else if	(buttontype == BUTTONTYPE::QUITMENU)
		button = "QUITMENU";
	else if	(buttontype == BUTTONTYPE::TRANSMENU)
		button = "TRANSMENU";
	else if	(buttontype == BUTTONTYPE::NONE)
		button = "";*/

	_jw.CreateKey(GetName());
	_jw.prettyWriterBuffer.StartObject();
	_jw.CreateStringMember("Script", scriptName);
	_jw.CreateStringMember("smName", smName);
	/*if (strcmp(button.c_str(), "") != 0)
		_jw.CreateStringMember("ButtonType", button);*/
	_jw.prettyWriterBuffer.EndObject();
}

void LogicComponent::Destroy()
{
	LOGIC->RemoveLogicComponent(GetID());
	LOGIC->GetScript(scriptName)->Exit();
}

void LogicComponent::Update(float _dt)
{
	UNREFERENCED_PARAMETER(_dt);
}

std::string LogicComponent::GetSMName()
{
	return smName;
}

std::string LogicComponent::GetScriptName()
{
	return scriptName;
}

LogicComponent::BUTTONTYPE LogicComponent::GetButtonType()
{
	return buttontype;
}

void LogicComponent::SetButtonType(BUTTONTYPE button)
{
	buttontype = button;
}
