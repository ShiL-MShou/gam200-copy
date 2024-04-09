/************************************************************************************ /
/*
File Name:		WinConditionScript.h
Project Name:	WhereGotLate
Author(s):		Tay Qin Wen Alvin [100%]


Brief:			To handle and detect for win condition.


Copyright Information:
All content © 2019 DigiPen (SINGAPORE) Corporation, all rights reserved.
*/
/*************************************************************************************/

#pragma once
#include "Script.h"
#include <memory>
#include "NPCScript.h"
#include "SpriteComponent.h"


class WinConditionScript : public Script
{
	std::shared_ptr<NPCScript> myNPCScript;

	std::weak_ptr<SpriteComponent> objMarkerPtr;

public:
	void Update(float _dt) override;

	void Start() override;

	void Init() override;

	void Exit() override;

	void LoadResource() override;

};

