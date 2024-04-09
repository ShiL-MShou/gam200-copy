/*************************************************************************************/
/*
File Name:		AlarmClockScript.h
Project Name:	WhereGotLate
Author(s):		Kok Zhe Yan			 [100%]


Copyright Information:
All content © 2019 DigiPen (SINGAPORE) Corporation, all rights reserved.
*/
/*************************************************************************************/
#pragma once
#include "Script.h"
#include "InteractablesFeedbackScript.h"

class AlarmClockScript : public Script
{
	std::weak_ptr<GOC> gameObjPtr; // Obj pointer
	
	// tmp, not used yet
	bool isDocked;

	InteractablesFeedbackScript		interactableFeedback;

public:
	void Update(float _dt) override;

	void Start() override;

	void Exit() override;

	void UseItem(const VEC2& pos, int frame) override;

	void SetOwner(std::shared_ptr<GOC> _gameObjPtr) override;

	void LoadResource() override;
};

