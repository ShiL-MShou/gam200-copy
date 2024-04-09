/************************************************************************************ /
/*
File Name:		AnimationManager.cpp
Project Name:	WhereGotLate
Author(s):		Tay Qin Wen Alvin [100%]


Brief:			This holds all the animation component and manages them.


Copyright Information:
All content © 2019 DigiPen (SINGAPORE) Corporation, all rights reserved.
*/
/*************************************************************************************/


#include "stdafx.h"
#include "AnimationManager.h"

#include "Graphics.h"
#include "Animation.h"
#include "CoreEngine.h"


AnimationManager::AnimationManager()
{
}

AnimationManager::~AnimationManager()
{
}

void AnimationManager::Update(float _dt)
{
	UNREFERENCED_PARAMETER(_dt);

	//for (int step = 0; step < CORE->currNumSteps; ++step)
	for (auto animationCompPtr : animationCompList)
	{
		std::shared_ptr<Animation> animationPtr = GRAPHICS->GetAnimation(animationCompPtr->animationList.at(animationCompPtr->currAnimation));
		animationPtr->SetOwner(animationCompPtr->GetBase());
		animationPtr->Update(CORE->GetDT());
	}
}

void AnimationManager::Initialize()
{
}

void AnimationManager::Clear()
{
	animationCompList.clear();

}

void AnimationManager::AddAnimationComponent(std::shared_ptr<AnimationComponent> _animationCompPtr)
{
	animationCompList.push_back(_animationCompPtr);
}

bool AnimationManager::IsLastFrame(std::shared_ptr<AnimationComponent> _animationCompPtr)
{
	std::shared_ptr<Animation> animationPtr = GRAPHICS->GetAnimation(_animationCompPtr->animationList.at(_animationCompPtr->currAnimation));

	return _animationCompPtr->currFrame == animationPtr->lastFrame;
}

void AnimationManager::RemoveComponent(std::shared_ptr<AnimationComponent> _compPtr)
{
	for (auto itr = std::begin(animationCompList); itr != std::end(animationCompList); ++itr)
	{
		if ((*itr) == _compPtr)
		{
			animationCompList.erase(itr);
			break;
		}
	}
}
