/************************************************************************************ /
/*
File Name:		Animation.cpp
Project Name:	WhereGotLate
Author(s):		Tay Qin Wen Alvin [100%]


Brief:			Animation holds all the information of its frame.


Copyright Information:
All content © 2019 DigiPen (SINGAPORE) Corporation, all rights reserved.
*/
/*************************************************************************************/


#include "stdafx.h"
#include "Animation.h"

Animation::Animation()
	: frames{}, startFrame{ 0 }, lastFrame{ 0 }, name{ "Animation Name" }
{
}

Animation::~Animation()
{
	frames.clear();
}

void Animation::SetOwner(std::shared_ptr<GOC> _owner)
{
	animationCompPtr = _owner->has(AnimationComponent);
}

void Animation::Update(float _dt)
{
	if (std::shared_ptr<AnimationComponent> tmpAnimationCompPtr = animationCompPtr.lock())
	{
		tmpAnimationCompPtr->currFrameTime -= _dt;

		if (tmpAnimationCompPtr->currFrameTime <= 0)
		{
			// update curr frame
			++tmpAnimationCompPtr->currFrame;

			// check for out of range
			if (tmpAnimationCompPtr->currFrame > lastFrame)
			{
				if (tmpAnimationCompPtr->repeat == true)
					tmpAnimationCompPtr->currFrame = 0;
				else
					tmpAnimationCompPtr->currFrame = lastFrame;
			}

			// set new frame time
			tmpAnimationCompPtr->currFrameTime = frames.at(tmpAnimationCompPtr->currFrame).timeDelay;
		}
	}
}


