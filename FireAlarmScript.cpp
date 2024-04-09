/*****************************************************************************/
/*
File Name:		FireAlarmScript.cpp
Project Name:	WhereGotLate
Author(s):		Ong Cheng Kai Marcus [80%]
				Kok Zhe Yan	 [20%]

Copyright information:
All content copyright 2020 DigiPen (SINGAPORE) Corporation, all rights reserved.
*/
/*****************************************************************************/
#include "stdafx.h"

#include "Logic.h"
#include "FireAlarmScript.h"
#include "LogicComponent.h"
#include "TextScript.h"
#include "CoreEngine.h"
#include "FModSoundManager.h"

//bool inFireAlarmDialog;

void FireAlarmScript::Update(float _dt)
{
	UNREFERENCED_PARAMETER(_dt);
	if (std::shared_ptr<GOC> firealarm = gameObjPtr.lock())
	{
		if (firealarm->has(BoxCollider)->IsCollided() && isActivated == false)
		{
			// creates interactable feedback
			//interactableFeedback.Create(gameObjPtr.lock()->has(Transform)->GetPosition() + VEC2(0.0f, 45.0f));

			if (INPUTMGR->GetKeyRelease(MVK_E))
			{
				isActivated = true;

				MyResource.RemoveObject(sparkles);

				/*SOUNDMGR->PlaySoundLoopFromMap("alarm");
				SOUNDMGR->PauseSoundFromMap("BGM");*/
				MyEvent.ActivateDistraction(gameObjPtr.lock()->has(BoxCollider)->GetCenterCoords(), "alarm");
				/*NPCDISTRACT message(gameObjPtr.lock()->has(BoxCollider)->GetCenterCoords());
				LOGIC->SendMessage(message);*/

				//CORE->TogglePausedStatus();
				//inFireAlarmDialog = true;
				/////
				//score
				//scoreNum += 50; 

				firealarm->has(BoxCollider)->SetCanCollide(false);
				
			}
			
			// reset collider
			firealarm->has(BoxCollider)->ResetIsCollide();
		}
		else
		{
			// destroys interactables feedback
			//interactableFeedback.Destroy();
		}

		//interactableFeedback.Update(_dt);
	}
	
}

void FireAlarmScript::Start()
{
}

void FireAlarmScript::Init()
{
	isActivated = false;
	pageCount = maxCount;

	std::shared_ptr<GOC> obj = gameObjPtr.lock();

	sparkles = LOGIC->CreateObjectAt(obj->has(Transform)->GetPosition() + VEC2{ -15.0f, 20.0f }, 0.0f, "InteractableSparkles", "Sparkles" + std::to_string(obj->GetId()))->GetId();


}

void FireAlarmScript::Exit()
{
	pageCount = maxCount;
}

void FireAlarmScript::LoadResource()
{
	// loading resources
	//FACTORY->CreateArcheType("AlarmVFX");

	//interactableFeedback.LoadResource();
	/*FACTORY->CreateArcheType("PlayerSprite");
	FACTORY->CreateArcheType("TeacherSprite");
	FACTORY->CreateArcheType("Tint");*/
}

//void FireAlarmScript::SetOwner(std::shared_ptr<GOC> _gameObjPtr)
//{
//	gameObjPtr = _gameObjPtr;
//}
