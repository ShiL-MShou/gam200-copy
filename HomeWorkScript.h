/*****************************************************************************/
/*
File Name:		HomeWorkScript.h
Project Name:	WhereGotLate
Author(s):		Kok Zhe Yan     	 [100%]

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


class HomeWorkScript : public Script
{
	//std::weak_ptr<GOC> gameObjPtr; // Obj pointer

	bool isActivated;
	//int pageCount;
	//const int maxCount = 6;
	std::weak_ptr<GOC> textObjPtr;
	std::weak_ptr<GOC> boyObjPtr;
	std::weak_ptr<GOC> boySpritePtr;
	std::weak_ptr<GOC> teacherObjPtr;
	std::weak_ptr<GOC> tintObjPtr;
	bool playHWText;
	InteractablesFeedbackScript		interactableFeedback;


public:
	void Update(float _dt) override;

	void Start() override;

	void Init() override;

	void Exit() override;

	//void SetOwner(std::shared_ptr<GOC> _gameObjPtr) override;

	void LoadResource() override;
};

extern bool HWcopied;