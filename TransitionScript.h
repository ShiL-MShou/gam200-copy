/************************************************************************************ /
/*
File Name:		TransitionScript.cpp
Project Name:	WhereGotLate
Author(s):		Tay Qin Wen Alvin [100%]

Brief:			Handling the fading in and out of the screen before moving
				into another scene.

Copyright Information:
All content © 2020 DigiPen (SINGAPORE) Corporation, all rights reserved.
*/
/*************************************************************************************/

#pragma once
#include "Script.h"


class TransitionScript : public Script
{
	std::string _transitionStr;

	float alpha;

public:
	TransitionScript();
	~TransitionScript();

	void Update(float _dt) override;

	void SetTransition(std::string _str) { _transitionStr = _str; }
};

