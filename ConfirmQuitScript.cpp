/*************************************************************************************/
/*
File Name:		ConfirmQuitScript.cpp
Project Name:	WhereGotLate
Author(s):		Kok Zhe Yan			 [100%]


Copyright Information:
All content © 2019 DigiPen (SINGAPORE) Corporation, all rights reserved.
*/
/*************************************************************************************/
#include "stdafx.h"
#include "ConfirmQuitScript.h"
#include "FModSoundManager.h"
#include "Logic.h"
#include "Input.h"

#include "SpriteComponent.h"


void ConfirmQuitScript::Start()
{
	
}

void ConfirmQuitScript::Update(float _dt)
{
	UNREFERENCED_PARAMETER(_dt);
	if (!isActive)
	{
		isActive = true;
		MyEvent.inConfirmQuit = true;
		std::shared_ptr<GOC> buttonno = LOGIC->CreateObjectAt(VEC2{}, 0.0f, "ButtonNo", "ButtonNo");
		std::shared_ptr<SpriteComponent> spritePtr = buttonno->has(SpriteComponent);
		spritePtr->SetStartFrame(2);
		spritePtr->SetCurrFrame(2);
		spritePtr->SetFixedPosition(VEC2{ 825.0f, 300.0f });
		//spritePtr->SetActive(false);
		mObjList.push_back(buttonno->GetId());

		std::shared_ptr<GOC> buttonyes = LOGIC->CreateObjectAt(VEC2{}, 0.0f, "ButtonYes", "ButtonYes");
		spritePtr = buttonyes->has(SpriteComponent);
		spritePtr->SetStartFrame(1);
		spritePtr->SetCurrFrame(1);
		spritePtr->SetFixedPosition(VEC2{ 475.0f, 300.0f });
		//spritePtr->SetActive(false);
		mObjList.push_back(buttonyes->GetId());
	}
}

void ConfirmQuitScript::Exit()
{
	mObjList.clear();
}

void ConfirmQuitScript::LoadResource()
{
	FACTORY->CreateArcheType("ConfirmQuit");
	FACTORY->CreateArcheType("ButtonNo");
	FACTORY->CreateArcheType("ButtonYes");
}

void ConfirmQuitScript::Init()
{
	
}

void ConfirmQuitScript::DestroyMenu()
{
	for (GOCId id : mObjList)
	{
		MyResource.RemoveObject(id);
		//MyResource.GetObjectWithID(id)->Destroy();
	}
	//GetOwner().lock()->has(SpriteComponent)->SetActive(false);
	GetOwner().lock()->Destroy();
	isActive = false;
	MyEvent.inConfirmQuit = false;
	
}


