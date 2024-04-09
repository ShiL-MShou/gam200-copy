/*************************************************************************************/
/*
File Name:		WinScript.h
Project Name:	WhereGotLate
Author(s):		Ong Cheng Kai Marcus [90%]
				Tay Qin Wen Alvin 	 [10%]

Copyright Information:
All content copyright 2020 DigiPen (SINGAPORE) Corporation, all rights reserved.
*/
/*************************************************************************************/
#pragma once

#include "Script.h"

#include "SpriteComponent.h"


class WinScript : public Script
{
	bool isParticlesInit;

	//dialogs
	std::weak_ptr<GOC>				textObjPtr; // Obj pointer
	std::weak_ptr<GOC>				boySpritePtr;
	std::weak_ptr<GOC>				tintPtr;
	std::weak_ptr<GOC>				gradePtr;

	std::weak_ptr<SpriteComponent>		reportCardPtr;
	std::weak_ptr<SpriteComponent>		sweePtr;
	bool WinDialog;
	bool ShowScore;
	bool playedSound;
public:

	WinScript();
	~WinScript();

	virtual void Start() override;
	virtual void Init() override;
	virtual void Update(float _dt) override;
	virtual void Exit() override;
	void LoadResource() override;

};

extern bool inWinDialog;
