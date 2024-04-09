/******************************************************************************/
/*
	File Name: IDLE_STATE.cpp
	Project Name: WhereGotLate
	Author(s): Lee Shi Lin [100%]

	Brief: Derived state for Teacher NPC

	Copyright Information:
	All content © 2019 DigiPen (SINGAPORE) Corporation, all rights reserved.
*/
/******************************************************************************/
#include "stdafx.h"
#include "IDLE_STATE.h"
#include "SpriteComponent.h"
#include "AnimationComponent.h"
#include "Graphics.h"
#include "StateMachine.h"


IDLE_STATE::IDLE_STATE()
	: Distract_des{}
{
	iState::_name = "Idle";
}

IDLE_STATE::~IDLE_STATE()
{
}

void IDLE_STATE::OnEnter()
{
	std::shared_ptr<SpriteComponent> sprPtr = GetOwner()->has(SpriteComponent);
	std::shared_ptr<AnimationComponent> animationCompPtr = GetOwner()->has(AnimationComponent);

	// safety checks
	if (sprPtr != nullptr && animationCompPtr != nullptr)
	{
		// change idle to teaching
		int animationID = animationCompPtr->animationList.at(animationCompPtr->currAnimation);
		std::string animationName = GRAPHICS->GetAnimationName(animationID);

		if (animationName.find("FALLING") != std::string::npos ||
			animationName.find("STUNNED") != std::string::npos ||
			animationName.find("GETTING_UP") != std::string::npos)
		{
			return;
		}
	}


	// Run idle animation
//	std::shared_ptr<SpriteComponent> sprPtr = GetOwner()->has(SpriteComponent);

	if (sprPtr != nullptr || IsUnableToChase() == false)
	{
		//bool tmpIsRight = GetOwner()->has(Transform)->GetScale().x > 0;

		sprPtr->ChangeGenericAnimation("IDLE");
		_ownerSM->updatedSprite = true;

		//if (!tmpIsRight)
		//{
		//	sprPtr->FlipSprite();
		//}
	}
}

void IDLE_STATE::Update(float _dt)
{
	UNREFERENCED_PARAMETER(_dt);

	//std::cout << "IDLE" << std::endl;

	if (IsUnableToChase() == true)
	{
		DisableAllForces(GetOwner()->has(Body));
		return;
	}

	//std::shared_ptr<SpriteComponent> sprPtr = GetOwner()->has(SpriteComponent);
	//std::shared_ptr<AnimationComponent> animationCompPtr = GetOwner()->has(AnimationComponent);

	//// safety checks
	//if (sprPtr != nullptr && animationCompPtr != nullptr)
	//{
	//	// change idle to teaching
	//	int animationID = animationCompPtr->animationList.at(animationCompPtr->currAnimation);
	//	std::string animationName = GRAPHICS->GetAnimationName(animationID);
	//	
	//	
	//	
	//	if (animationName.find("IDLE") != std::string::npos)
	//	{
	//		//bool tmpIsRight = GetOwner()->has(Transform)->GetScale().x > 0;

	//		//sprPtr->ChangeGenericAnimation("TEACHING");

	//		//if (tmpIsRight)
	//		//{
	//		//	sprPtr->FlipSprite();
	//		//}
	//	}
	//	//else if (animationName.find("TEACHING") != std::string::npos)
	//	//{
	//	//	// reached last frame
	//	//	if (animationCompPtr->currFrame == GRAPHICS->GetAnimation(animationID)->lastFrame)
	//	//	{
	//	//
	//	//	}
	//	//}
	//}

}

void IDLE_STATE::UpdateAnimation()
{
	if (IsUnableToChase() == true)
		return;

	GetOwner()->has(SpriteComponent)->ChangeGenericAnimation("IDLE");
	_ownerSM->updatedSprite = true;
}

void IDLE_STATE::SetDistractDes(VEC2 t)
{
	Distract_des = t;
}

VEC2 IDLE_STATE::GetDistractDes()
{
	return Distract_des;
}

