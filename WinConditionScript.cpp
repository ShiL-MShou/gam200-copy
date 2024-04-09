/************************************************************************************ /
/*
File Name:		WinConditionScript.cpp
Project Name:	WhereGotLate
Author(s):		Tay Qin Wen Alvin [90%]
				Lee Shi Lin [10%]


Brief:			To handle and detect for win condition.


Copyright Information:
All content © 2020 DigiPen (SINGAPORE) Corporation, all rights reserved.
*/
/*************************************************************************************/


#include "stdafx.h"
#include "CoreEngine.h"
#include "WinConditionScript.h"
#include "BoxCollider.h"
#include "Logic.h"
#include "FModSoundManager.h"


void WinConditionScript::Update(float _dt)
{
	UNREFERENCED_PARAMETER(_dt);
	std::shared_ptr<BoxCollider> boxColliderPtr = gameObjPtr.lock()->has(BoxCollider);

	if (boxColliderPtr->IsCollided() && boxColliderPtr->GetCollidedType() == CollideTypes::PLAYER)
	{
		if (MyEvent.GetCurrEvent() != EventManager::NPCEvent::CHASE)
		{
			objMarkerPtr.lock()->SetActive(false);
			Message msg(MessageId::Win);
			LOGIC->SendMessageW(msg);
		}
		
	}
	boxColliderPtr->ResetIsCollide();	
}

void WinConditionScript::Start()
{
}

void WinConditionScript::Init()
{
	if (strcmp("Stage1v2", CORE->GetCurrLevel().c_str()) == 0)
	{
		objMarkerPtr = LOGIC->CreateObjectAt(VEC2(1550, 850), 0, "ObjectiveMarker", "objm1", false)->has(SpriteComponent);
	}
	else if (strcmp("Stage2v2", CORE->GetCurrLevel().c_str()) == 0) 
	{
		objMarkerPtr = LOGIC->CreateObjectAt(VEC2(1200, 900), 0, "ObjectiveMarker", "objm1", false)->has(SpriteComponent);
	}
	else if (strcmp("Stage3v2", CORE->GetCurrLevel().c_str()) == 0)
	{
		objMarkerPtr = LOGIC->CreateObjectAt(VEC2(2100, 900), 0, "ObjectiveMarker", "objm1", false)->has(SpriteComponent);
	}
	else if (strcmp("Stage4", CORE->GetCurrLevel().c_str()) == 0)
	{
		objMarkerPtr = LOGIC->CreateObjectAt(VEC2(565, 2200), 0, "ObjectiveMarker", "objm1", false)->has(SpriteComponent);
	}
	else
	{
		objMarkerPtr = LOGIC->CreateObjectAt(VEC2(1200, 1000), 0, "ObjectiveMarker", "objm1", false)->has(SpriteComponent);
	}
	
	objMarkerPtr.lock()->SetActive(true);
}

void WinConditionScript::Exit()
{
}

void WinConditionScript::LoadResource()
{
	FACTORY->CreateArcheType("ObjectiveMarker");
}