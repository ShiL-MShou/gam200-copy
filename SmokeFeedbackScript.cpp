/************************************************************************************ /
/*
File Name:		SmokeFeedbackScript.cpp
Project Name:	WhereGotLate
Author(s):		Tay Qin Wen Alvin [100%]


Brief:			Smoke feedback for particles.


Copyright Information:
All content © 2019 DigiPen (SINGAPORE) Corporation, all rights reserved.
*/
/*************************************************************************************/


#include "stdafx.h"
#include "SmokeFeedbackScript.h"

// create obj
#include "Logic.h"

// "load" Archetype
#include "Factory.h"


SmokeFeedbackScript::SmokeFeedbackScript()
{
}

SmokeFeedbackScript::~SmokeFeedbackScript()
{
}

void SmokeFeedbackScript::Create(VEC2 _pos)
{
	// create if it is not created already
	if (feedbackObjPtr.lock() == nullptr)
	{
		feedbackObjPtr = LOGIC->CreateObjectAt(_pos, 0.0f, "SmokeFeedback", "SmokeFeedback");
	}
}

void SmokeFeedbackScript::Destroy()
{
	// safety check
	if (feedbackObjPtr.lock())
	{
		feedbackObjPtr.lock()->Destroy();
	}
}

void SmokeFeedbackScript::Update(float _dt)
{
	UNREFERENCED_PARAMETER(_dt);

	// safety check
	if (feedbackObjPtr.lock())
	{
		std::shared_ptr<AnimationComponent> animationCompPtr = feedbackObjPtr.lock()->has(AnimationComponent);

		// destroy once it reaches the last frame
		if (animationCompPtr->currFrame == (GRAPHICS->GetAnimation(animationCompPtr->animationList.at(0))->lastFrame - 1))
		{
			feedbackObjPtr.lock()->Destroy();
		}
	}
}

void SmokeFeedbackScript::LoadResource()
{
	FACTORY->CreateArcheType("SmokeFeedback");
}
