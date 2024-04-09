/*************************************************************************************/
/*
File Name:		AlarmClockScript.cpp
Project Name:	WhereGotLate
Author(s):		Kok Zhe Yan			 [100%]


Copyright Information:
All content © 2019 DigiPen (SINGAPORE) Corporation, all rights reserved.
*/
/*************************************************************************************/
#include "stdafx.h"

#include "Input.h"
#include "Logic.h"
#include "AlarmClockScript.h"
#include "SpriteComponent.h"


void AlarmClockScript::Start()
{
}

void AlarmClockScript::Update(float _dt)
{
	UNREFERENCED_PARAMETER(_dt);
	if (std::shared_ptr<GOC> clock = gameObjPtr.lock())
	{
		std::shared_ptr<BoxCollider> colliderptr = clock->has(BoxCollider);
		if (colliderptr->IsCollided() && colliderptr->GetCollidedType() == CollideTypes::NPC)
			clock->Destroy();

		//if (colliderptr->IsCollided() && colliderptr->GetCollidedType() == CollideTypes::PLAYER)
		//{
		//	interactableFeedback.Create(gameObjPtr.lock()->has(Transform)->GetPosition() + VEC2(0.0f, 45.0f));
		//	colliderptr->ResetIsCollide();
		//}
		//else
		//{
		//	// destroys interactables feedback
		//	interactableFeedback.Destroy();
		//}

		//std::shared_ptr<BoxCollider> colliderPtr = clock->has(BoxCollider);
		//if (colliderPtr->IsCollided() && colliderPtr->GetCollidedType() == CollideTypes::PLAYER && INPUTMGR->GetKeyPressed(MVK_E))
		//{

		//}
		//if (colliderPtr->IsCollided() && colliderPtr->GetCollidedType() == CollideTypes::NPC)
		//{
		//	//LOGIC->getAlarmSoundptr()->playSoundLoop();
		//	/*NPCDISTRACT message(clock->has(Transform)->GetPosition());
		//	LOGIC->SendMessage(message);*/

		//	//textObjPtr = LOGIC->CreateObjectAt(VEC2(0, 0), 0, "Text", "Fire Alarm Text");
		//	//std::dynamic_pointer_cast<TextScript>(LOGIC->GetScript(textObjPtr.lock()->has(LogicComponent)->GetScriptName()))->Serialize(CORE->mPath + "Text/Text.json");
		//}
	}
	interactableFeedback.Update(_dt);
}

void AlarmClockScript::Exit()
{
}

void AlarmClockScript::UseItem(const VEC2& pos, int frame)
{
	UNREFERENCED_PARAMETER(frame);
	if (std::shared_ptr<GOC> clock = gameObjPtr.lock())
	{
		std::shared_ptr<GOC> obj = LOGIC->CreateObjectAt(pos, 0, "AlarmClockRinging", "Clock");
		obj->has(SpriteComponent)->SetCurrFrame(8);
		obj->has(Transform)->SetScale(obj->has(Transform)->GetScale() * 0.5f);
		//obj->has(Sprite)->SetDepth(0.21f);

		/*NPCDISTRACT message(clock->has(Transform)->GetPosition());*/
		NPCDISTRACT message(pos, false);
		LOGIC->SendMessage(message);
	}
}

void AlarmClockScript::SetOwner(std::shared_ptr<GOC> _gameObjPtr)
{
	gameObjPtr = _gameObjPtr;
}

void AlarmClockScript::LoadResource()
{
	FACTORY->CreateArcheType("AlarmClockRinging");
}
