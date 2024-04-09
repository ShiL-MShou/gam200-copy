/************************************************************************************ /
/*
File Name:		WaterCoolerScript.g
Project Name:	WhereGotLate
Author(s):		Tay Qin Wen Alvin [100%]

Brief:			Script for water cooler. (NOT USED)


Copyright Information:
All content © 2020 DigiPen (SINGAPORE) Corporation, all rights reserved.
*/
/*************************************************************************************/

#pragma once
#include "Script.h"

#include "InteractablesFeedbackScript.h"
#include <vector>
#include <memory>


class WaterCoolerScript :	public Script
{
	bool isWorking;
	std::vector<std::weak_ptr<GOC>> waterList;

	//InteractablesFeedbackScript		interactableFeedback;

public:
	WaterCoolerScript();
	~WaterCoolerScript();

	WaterCoolerScript(const WaterCoolerScript&) = delete;
	WaterCoolerScript& operator=(const WaterCoolerScript&) = delete;

	void Init() override;

	void Start() override;

	void Update(float _dt) override;
	
	void Exit() override;

	//void SetOwner(std::shared_ptr<GOC> _gameObjPtr) override;

	void LoadResource() override;
};
