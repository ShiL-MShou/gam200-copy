/************************************************************************************ /
/*
File Name:		LogicComponent.h
Project Name:	WhereGotLate
Author(s):		Tay Qin Wen Alvin [100%]


Brief:			Logic component.


Copyright Information:
All content © 2019 DigiPen (SINGAPORE) Corporation, all rights reserved.
*/
/*************************************************************************************/

#pragma once

#include "Composition.h"
#include "Script.h"

#include "PlayerScript.h"

class LogicComponent : public Component
{
public:

	enum class BUTTONTYPE
	{
		MAINMENU,
		PAUSEMENU,
		QUITMENU,
		TRANSMENU,
		NONE
	};

	LogicComponent();
	~LogicComponent();

	void Initialize() override;
	void Inspect() override;

	std::shared_ptr<Component> Clone() override;
	void Serialize(ISerialization& stream) override;
	void Serialize(JsonReader& _jr) override;
	void DeSerialize(JsonWriter& _jw) override;

	void Destroy() override;

	void Update(float _dt);

	std::string GetSMName();
	std::string GetScriptName();
	BUTTONTYPE GetButtonType();
	void SetButtonType(BUTTONTYPE button);

private:

	std::string smName;

	std::string scriptName;

	bool isCollided;

	BUTTONTYPE buttontype;

};

