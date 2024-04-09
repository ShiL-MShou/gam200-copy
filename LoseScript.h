/************************************************************************************ /
/*
File Name:		LoseScript.h
Project Name:	WhereGotLate
Author(s):		Tay Qin Wen Alvin [100%]


Brief:			Script for lose condition.

Copyright Information:
All content © 2019 DigiPen (SINGAPORE) Corporation, all rights reserved.
*/
/*************************************************************************************/

#pragma once

#include "Script.h"


class LoseScript : public Script
{
	bool isParticlesInit;
	bool soundsPlayed;
	std::weak_ptr<GOC> gradePtr;
	std::weak_ptr<GOC> npcPtr;

public:
	LoseScript();
	~LoseScript();

	virtual void Start() override;
	virtual void Update(float _dt) override;
	virtual void Exit() override;
	void LoadResource() override;
};
