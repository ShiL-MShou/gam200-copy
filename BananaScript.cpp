/*************************************************************************************/
/*
File Name:		BananaScript.cpp
Project Name:	WhereGotLate
Author(s):		Kok Zhe Yan			 [100%]


Copyright Information:
All content © 2019 DigiPen (SINGAPORE) Corporation, all rights reserved.
*/
/*************************************************************************************/
#include "stdafx.h"

#include "BananaScript.h"
#include "CoreEngine.h"
#include "TextScript.h"
#include "Logic.h"
#include "Factory.h"

#include "SpriteComponent.h"


void BananaScript::Update(float _dt)
{
	UNREFERENCED_PARAMETER(_dt);

	//if (std::shared_ptr<GOC> banana = gameObjPtr.lock())
	//{
	//	std::shared_ptr<BoxCollider> colliderptr = banana->has(BoxCollider);
	//	
	//	if (colliderptr->IsCollided() && colliderptr->GetCollidedType() == CollideTypes::PLAYER)
	//	{
	//		interactableFeedback.Create(gameObjPtr.lock()->has(Transform)->GetPosition() + VEC2(0.0f, 45.0f));
	//		colliderptr->ResetIsCollide();
	//	}
	//	else
	//	{
	//		// destroys interactables feedback
	//		interactableFeedback.Destroy();
	//	}
	//
	//	interactableFeedback.Update(_dt);
	//
	//	if (colliderptr->IsCollided() && colliderptr->GetCollidedType() == CollideTypes::NPC)
	//		banana->Destroy();
	//}
	//interactableFeedback.Update(_dt);
}

void BananaScript::Start()
{
}

void BananaScript::Exit()
{
}

void BananaScript::UseItem(const VEC2& pos, int frame)
{
	if (std::shared_ptr<GOC> clock = gameObjPtr.lock())
	{
		std::shared_ptr<GOC> obj = LOGIC->CreateObjectAt(pos, 0, "Banana", "Banana");
		obj->has(SpriteComponent)->SetCurrFrame(frame);
		obj->has(Transform)->SetScale(obj->has(Transform)->GetScale() * 0.5f);
		obj->has(SpriteComponent)->SetDepth(0.21f);
	}
}

void BananaScript::SetOwner(std::shared_ptr<GOC> _gameObjPtr)
{
	gameObjPtr = _gameObjPtr;
}
