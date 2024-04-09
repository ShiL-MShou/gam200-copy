/*************************************************************************************/
/*
File Name:		RubbishBin.cpp
Project Name:	WhereGotLate
Author(s):		Kok Zhe Yan			 [100%]


Copyright Information:
All content © 2019 DigiPen (SINGAPORE) Corporation, all rights reserved.
*/
/*************************************************************************************/
#include "stdafx.h"

#include "Input.h"
#include "Logic.h"
#include "RubbishBin.h"
//#include "SpriteComponent.h"
#include "ResourceManager.h"
#include "PlayerScript.h"


void RubbishBin::Start()
{
}

void RubbishBin::Init()
{
	std::shared_ptr<GOC> obj = gameObjPtr.lock();
	emptybins[obj->GetId()] = LOGIC->CreateObjectAt(obj->has(Transform)->GetPosition() + VEC2{-15.0f, 20.0f}, 0.0f, "InteractableSparkles", "Sparkles" + std::to_string(obj->GetId()))->GetId();
}



void RubbishBin::Update(float _dt)
{
	UNREFERENCED_PARAMETER(_dt);
	if (INPUTMGR->GetKeyRelease(MVK_E))
	{
		if (std::shared_ptr<GOC> bin = gameObjPtr.lock())
		{
			std::shared_ptr<BoxCollider> colliderptr = bin->has(BoxCollider);

			if (colliderptr->IsCollided() && colliderptr->GetCollidedType() == CollideTypes::PLAYER && !isInventoryFull)
			{
				MyResource.RemoveObject(emptybins[bin->GetId()]);
				emptybins[bin->GetId()] = 0;
				colliderptr->SetCanCollide(false);
				colliderptr->ResetIsCollide();
				isInventoryFull = true;
			}
		}
	}

}

void RubbishBin::Exit()
{
}

void RubbishBin::SetOwner(std::shared_ptr<GOC> _gameObjPtr)
{
	gameObjPtr = _gameObjPtr;
}

void RubbishBin::LoadResource()
{
	FACTORY->CreateArcheType("InteractableSparkles");
}
