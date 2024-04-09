/************************************************************************************ /
/*
File Name:		InteractablesFeedbackScript.cpp
Project Name:	WhereGotLate
Author(s):		Tay Qin Wen Alvin [100%]


Brief:			Interactable feedback for particles.


Copyright Information:
All content © 2019 DigiPen (SINGAPORE) Corporation, all rights reserved.
*/
/*************************************************************************************/

#include "stdafx.h"
#include "InteractablesFeedbackScript.h"

// to create object
#include "Logic.h"


InteractablesFeedbackScript::InteractablesFeedbackScript()
{
}

InteractablesFeedbackScript::~InteractablesFeedbackScript()
{
}

void InteractablesFeedbackScript::Create(VEC2 _pos)
{
	// create if it is not created already
	if (feedbackObjPtr.lock() == nullptr)
	{
		feedbackObjPtr = LOGIC->CreateObjectAt(_pos, 0.0f, "InteractableFeedback", "InteractableFeedback");
	}
}

void InteractablesFeedbackScript::Destroy()
{
	// safety check
	if (feedbackObjPtr.lock())
	{
		feedbackObjPtr.lock()->Destroy();
	}
}

void InteractablesFeedbackScript::Update(float _dt)
{
	UNREFERENCED_PARAMETER(_dt);

	// safety check
	if (feedbackObjPtr.lock())
	{
		std::shared_ptr<AnimationComponent> animationCompPtr = feedbackObjPtr.lock()->has(AnimationComponent);

		// changes the animation if it is the first animation and it reaches the last frame
		if (animationCompPtr->currAnimation == 0 &&
			animationCompPtr->currFrame == (GRAPHICS->GetAnimation(animationCompPtr->animationList.at(0))->lastFrame - 1))
		{
			feedbackObjPtr.lock()->has(SpriteComponent)->ChangeAnimation("INTERACT_LOOP");
		}
			
	}
}

void InteractablesFeedbackScript::LoadResource()
{
	FACTORY->CreateArcheType("InteractableFeedback");
}

bool InteractablesFeedbackScript::IsActive()
{
	return (feedbackObjPtr.lock() != nullptr);
}
