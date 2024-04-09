/************************************************************************************ /
/*
File Name:		InteractablesFeedbackScript.h
Project Name:	WhereGotLate
Author(s):		Tay Qin Wen Alvin [100%]


Brief:			Interactable feedback for particles.


Copyright Information:
All content © 2019 DigiPen (SINGAPORE) Corporation, all rights reserved.
*/
/*************************************************************************************/

#pragma once
#include <memory>
#include "Composition.h"


class InteractablesFeedbackScript
{
	std::weak_ptr<GOC>	feedbackObjPtr;

public:
	InteractablesFeedbackScript();
	~InteractablesFeedbackScript();

	InteractablesFeedbackScript(const InteractablesFeedbackScript&) = delete;
	InteractablesFeedbackScript& operator=(const InteractablesFeedbackScript&) = delete;


	void Create(VEC2 _pos);
	void Destroy();

	void Update(float _dt);


	void LoadResource();

	bool IsActive();
};

