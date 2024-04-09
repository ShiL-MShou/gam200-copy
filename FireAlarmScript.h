/*****************************************************************************/
/*
File Name:		FireAlarmScript.h
Project Name:	WhereGotLate
Author(s):		Ong Cheng Kai Marcus [40%]
				Kok Zhe Yan     	 [60%]

Copyright information:
All content copyright 2020 DigiPen (SINGAPORE) Corporation, all rights reserved.
*/
/*****************************************************************************/
#pragma once
#include "Script.h"
#include <memory>
#include "Composition.h"
#include "Logic.h"
#include "Transform.h"
#include "InteractablesFeedbackScript.h"
#include "SpriteComponent.h"

class FireAlarmScript : public Script 
{
	//std::weak_ptr<GOC> gameObjPtr; // Obj pointer

	bool isActivated;
	int pageCount;
	const int maxCount = 6;
	std::weak_ptr<GOC> textObjPtr;
	std::weak_ptr<GOC> boyObjPtr;
	std::weak_ptr<GOC> teacherObjPtr;
	std::weak_ptr<GOC> tintObjPtr;

	//std::weak_ptr<SpriteComponent> alarmVFXPtr;

	GOCId sparkles;

public:
	void Update(float _dt) override;

	void Start() override;

	void Init() override;

	void Exit() override;

	//void SetOwner(std::shared_ptr<GOC> _gameObjPtr) override;

	void LoadResource() override;
};
extern bool inFireAlarmDialog;