/************************************************************************************ /
/*
File Name:		WaterCoolerScript.cpp
Project Name:	WhereGotLate
Author(s):		Tay Qin Wen Alvin [100%]

Brief:			Script for water cooler. (NOT USED)


Copyright Information:
All content © 2020 DigiPen (SINGAPORE) Corporation, all rights reserved.
*/
/*************************************************************************************/

#include "stdafx.h"
#include "WaterCoolerScript.h"
#include "Logic.h"
#include "Input.h"

#include <iterator>


WaterCoolerScript::WaterCoolerScript()
	: isWorking{ true }, waterList{}
{
}

WaterCoolerScript::~WaterCoolerScript()
{
}

void WaterCoolerScript::Init()
{
	isWorking = true;
}

void WaterCoolerScript::Start()
{
}

void WaterCoolerScript::Update(float _dt)
{
	UNREFERENCED_PARAMETER(_dt);

	std::shared_ptr<GOC> ownerPtr = gameObjPtr.lock();

	/*if (ownerPtr != nullptr)
	{
		if (ownerPtr->has(BoxCollider)->IsCollided() && isWorking == true)
		{
			// creates interactable feedback
			interactableFeedback.Create(gameObjPtr.lock()->has(Transform)->GetPosition() + VEC2(0.0f, 45.0f));

			// reset collider
			ownerPtr->has(BoxCollider)->ResetIsCollide();
		}
		else
		{
			// destroys interactables feedback
			interactableFeedback.Destroy();
		}

		interactableFeedback.Update(_dt);
	}*/

	if (isWorking == true)
	{
		if (std::shared_ptr<GOC> waterCooler = gameObjPtr.lock())
		{
			if (waterCooler->has(BoxCollider)->IsCollided())// && isActivated == false)
			{
				if (INPUTMGR->GetKeyPressed(MVK_E))
				{
					// creates water object.

					// to create one line as an objecct or multiple small objects?

					// to create multiple water puddles, until it reaches the bottom wall?

					//isWorking = false;

					int num = 2;

					VEC2 pos = ownerPtr->has(Transform)->GetPosition();

					pos.y -= 100;
					VEC2 offset(0, -64);
					for (int i = 0; i < num; ++i)
					{
						waterList.push_back(LOGIC->CreateObjectAt(pos + offset * static_cast<float>(i), 0, "WaterPuddle", "WaterPuddle" + std::to_string(i + 1)));
					}
				}
			}
		}
	}
	if (waterList.empty() == false)
	{
		for (auto itr = std::begin(waterList); itr != std::end(waterList); )
		{
			if ((*itr).lock() == nullptr)
			{
				itr = waterList.erase(itr);
			}
			else
			{
				std::shared_ptr<AnimationComponent> animationCompPtr = (*itr).lock()->has(AnimationComponent);
				int animationID = animationCompPtr->animationList.at(animationCompPtr->currAnimation);
				std::shared_ptr<Animation> animationPtr = GRAPHICS->GetAnimation(animationID);

				if (animationCompPtr->currFrame == animationPtr->lastFrame && animationCompPtr->currFrameTime <= 0.1f)
				{
					(*itr).lock()->Destroy();
					itr = waterList.erase(itr);
				}
				else
				{
					++itr;
				}
			}
		}
	}


}

void WaterCoolerScript::Exit()
{
}

void WaterCoolerScript::LoadResource()
{
	//water script?

	//creation of water script

	FACTORY->CreateArcheType("WaterPuddle");

}
