/*************************************************************************************/
/*
File Name:		MouseCursorScript.h
Project Name:	WhereGotLate
Author(s):		Kok Zhe Yan			 [100%]


Copyright Information:
All content © 2019 DigiPen (SINGAPORE) Corporation, all rights reserved.
*/
/*************************************************************************************/
#pragma once
#include "Script.h"

class MouseCursorScript : public Script
{
	std::weak_ptr<GOC> gameObjPtr; // Obj pointer

public:
	void Update(float _dt) override;

	void Start() override;

	void Exit() override;

	void SetOwner(std::shared_ptr<GOC> _gameObjPtr) override;
};

