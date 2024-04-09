/************************************************************************************ /
/*
File Name:		SmokeFeedbackScript.h
Project Name:	WhereGotLate
Author(s):		Tay Qin Wen Alvin [100%]


Brief:			Smoke feedback for particles.


Copyright Information:
All content © 2019 DigiPen (SINGAPORE) Corporation, all rights reserved.
*/
/*************************************************************************************/


#pragma once
#include <memory>
#include "Composition.h"


class SmokeFeedbackScript
{
	std::weak_ptr<GOC>	feedbackObjPtr;

public:
	SmokeFeedbackScript();
	~SmokeFeedbackScript();

	SmokeFeedbackScript(const SmokeFeedbackScript&) = delete;
	SmokeFeedbackScript& operator=(const SmokeFeedbackScript&) = delete;


	void Create(VEC2 _pos);
	void Destroy();

	void Update(float _dt);

	void LoadResource();
};

