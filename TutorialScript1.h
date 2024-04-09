/*****************************************************************************/
/*
File Name:		TutorialScript1.h
Project Name:	WhereGotLate
Author(s):		Ong Cheng Kai Marcus [100%]

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
class TutorialScript1 : public Script
{
	const int maxCount;
	int PageCount;

	std::weak_ptr<GOC> textObjPtr;
	std::weak_ptr<GOC> boyObjPtr;
	std::weak_ptr<GOC> tintObjPtr;
	//std::weak_ptr<SpriteComponent> barTopPtr;
public:
	TutorialScript1();
	~TutorialScript1();

	void Init() override;

	void Update(float _dt) override;

	void TextUpdate();

	void Start() override;

	void LoadResource() override;

	void Exit() override;
};

extern bool IntroDialogDone;
