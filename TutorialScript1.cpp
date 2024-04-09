/*****************************************************************************/
/*
File Name:		TutorialScript1.cpp
Project Name:	WhereGotLate
Author(s):		Ong Cheng Kai Marcus [80%]
				Lee Shi Lin [20%

Copyright information:
All content copyright 2020 DigiPen (SINGAPORE) Corporation, all rights reserved.
*/
/*****************************************************************************/
#include "stdafx.h"
#include "FModSoundManager.h"
#include "TutorialScript1.h"
#include "PlayerScript.h" //extern bool inDialog
#include "Logic.h"
#include "LogicComponent.h"
#include "TextScript.h"
#include "CoreEngine.h"
#include "Graphics.h"

 bool IntroDialogDone;

void TutorialScript1::Start()
{
	PageCount = maxCount;
	IntroDialogDone = false;
}

TutorialScript1::TutorialScript1(): maxCount{3}, PageCount{maxCount}
{

}

TutorialScript1::~TutorialScript1()
{
}

void TutorialScript1::Init()
{
	FACTORY->CreateArcheType("Text");
	FACTORY->CreateArcheType("PlayerSprite");
	FACTORY->CreateArcheType("Tint");
	//barTopPtr = LOGIC->CreateObjectAt(VEC2(0,0), 0, "BlackBarTop", "bb1", false)->has(SpriteComponent);
	//barTopPtr.lock()->SetActive(false);
}

void TutorialScript1::Update(float _dt)
{
	UNREFERENCED_PARAMETER(_dt);



	if ((PageCount == maxCount) && !IntroDialogDone)
	{
		CORE->SetPausedStatus(true);
		MyEvent.inGameStage = true;
		//textObjPtr = LOGIC->CreateObjectAt(VEC2(0, 0), 0, "Text", "Tutorial 1 Text");
		//boyObjPtr = LOGIC->CreateObjectAt(VEC2(0, 0), 0, "PlayerSprite", "PlayerDialog");
		//tintObjPtr = LOGIC->CreateObjectAt(VEC2(0, 0), 0, "Tint", "Tutorial 1 Text");
		MyEvent.textPtr.lock()->SetActive(true);
		MyEvent.boyPtr.lock()->SetActive(true);
		MyEvent.tintPtr.lock()->SetActive(true);
		

		SOUNDMGR->PlaySoundFromMap("boyVoice");

		if (strcmp(CORE->GetCurrLevel().c_str(), "Stage1v2") == 0)
			MyEvent.textscriptPtr.lock()->Serialize(CORE->mPath + "Text/Intro/Intro1.json");
		else if(strcmp(CORE->GetCurrLevel().c_str(), "Stage2v2") == 0)
			MyEvent.textscriptPtr.lock()->Serialize(CORE->mPath + "Text/Intro2/Intro1.json");
		else if (strcmp(CORE->GetCurrLevel().c_str(), "Stage3v2") == 0)
			MyEvent.textscriptPtr.lock()->Serialize(CORE->mPath + "Text/Intro3/Intro1.json");
		else if (strcmp(CORE->GetCurrLevel().c_str(), "Stage4") == 0)
			MyEvent.textscriptPtr.lock()->Serialize(CORE->mPath + "Text/Intro4/Intro1.json");
		--PageCount;//becomes 1
	}

	if(!IntroDialogDone)//if still in dialog
		if (INPUTMGR->GetKeyPressed(MVK_RETURN) || INPUTMGR->GetKeyPressed(MVK_SPACE) || INPUTMGR->GetKeyPressed(MVK_LBUTTON))
		{

			if (PageCount == 2)
			{
				SOUNDMGR->PlaySoundFromMap("boyYikes");
				if (strcmp(CORE->GetCurrLevel().c_str(), "Stage1v2") == 0)
					MyEvent.textscriptPtr.lock()->Serialize(CORE->mPath + "Text/Intro/Intro2.json");
				else if (strcmp(CORE->GetCurrLevel().c_str(), "Stage2v2") == 0)
					MyEvent.textscriptPtr.lock()->Serialize(CORE->mPath + "Text/Intro2/Intro2.json");
				else if (strcmp(CORE->GetCurrLevel().c_str(), "Stage3v2") == 0)
					MyEvent.textscriptPtr.lock()->Serialize(CORE->mPath + "Text/Intro3/Intro2.json");
				else if (strcmp(CORE->GetCurrLevel().c_str(), "Stage4") == 0)
					MyEvent.textscriptPtr.lock()->Serialize(CORE->mPath + "Text/Intro4/Intro2.json");


				--PageCount;
				return;
			}
			if (PageCount == 1)
			{
				SOUNDMGR->PlaySoundFromMap("boyVoice");
				if (strcmp(CORE->GetCurrLevel().c_str(), "Stage1v2") == 0)
					MyEvent.textscriptPtr.lock()->Serialize(CORE->mPath + "Text/Intro/Intro3.json");
				else if (strcmp(CORE->GetCurrLevel().c_str(), "Stage2v2") == 0)
					MyEvent.textscriptPtr.lock()->Serialize(CORE->mPath + "Text/Intro2/Intro3.json");
				else if (strcmp(CORE->GetCurrLevel().c_str(), "Stage3v2") == 0)
					MyEvent.textscriptPtr.lock()->Serialize(CORE->mPath + "Text/Intro3/Intro3.json");
				else if (strcmp(CORE->GetCurrLevel().c_str(), "Stage4") == 0)
					MyEvent.textscriptPtr.lock()->Serialize(CORE->mPath + "Text/Intro4/Intro3.json");
				--PageCount;
				return;
			}
			if (PageCount == 0)
			{
				CORE->SetPausedStatus(false);

				//if (std::shared_ptr<GOC> text = textObjPtr.lock())//checks if valid
				//	text->Destroy();
				//if (std::shared_ptr<GOC> tint = tintObjPtr.lock())//checks if valid
				//	tint->Destroy();
				//if (std::shared_ptr<GOC> boy = boyObjPtr.lock())
				//	boy->Destroy(); //instead of FACTORY->Destroy(text)
				MyEvent.textPtr.lock()->SetActive(false);
				MyEvent.boyPtr.lock()->SetActive(false);
				MyEvent.tintPtr.lock()->SetActive(false);

				IntroDialogDone = true;


				if (strcmp("Stage2v2", CORE->GetCurrLevel().c_str()) == 0)
				{
					GRAPHICS->GetMainCameraPtr()->AddToMoveList(VEC2(100, 100), 0, 0.5f);
					GRAPHICS->GetMainCameraPtr()->AddToMoveList(VEC2(900, 100), 0, 0.5f);
					GRAPHICS->GetMainCameraPtr()->AddToMoveList(VEC2(900, 1050), 0, 0.5f);
					GRAPHICS->GetMainCameraPtr()->AddToMoveList(VEC2(400, 1050), 0, 0.5f);
					GRAPHICS->GetMainCameraPtr()->AddToMoveList(VEC2(400, 550), 1, 0.5f);
					GRAPHICS->GetMainCameraPtr()->AddFollowPos(0.5f);
				}
				else if (strcmp("Stage1v2", CORE->GetCurrLevel().c_str()) == 0)
				{
					//barTopPtr.lock()->SetActive(true);
					GRAPHICS->GetMainCameraPtr()->AddToMoveList(VEC2(-50, 1200), 0, 0.5f);
					GRAPHICS->GetMainCameraPtr()->AddToMoveList(VEC2(900, 1200), 1, 0.5f);
					GRAPHICS->GetMainCameraPtr()->AddToMoveList(VEC2(900, 540), 1, 0.5f);
					GRAPHICS->GetMainCameraPtr()->AddFollowPos(0.5f);
					//barTopPtr.lock()->SetActive(false);
				}
				else if (strcmp("Stage3v2", CORE->GetCurrLevel().c_str()) == 0)
				{
					GRAPHICS->GetMainCameraPtr()->AddToMoveList(VEC2(-100, 1900), 0.5, 0.5f);
					GRAPHICS->GetMainCameraPtr()->AddToMoveList(VEC2(300, 1070), 0.5, 0.5f);
					GRAPHICS->GetMainCameraPtr()->AddToMoveList(VEC2(1100, 1650), 1, 0.5f);
					GRAPHICS->GetMainCameraPtr()->AddToMoveList(VEC2(1300, 500), 1, 0.5f);

					GRAPHICS->GetMainCameraPtr()->AddFollowPos(0.5f);
				}
				else if (strcmp("Stage4", CORE->GetCurrLevel().c_str()) == 0)
				{
					// Grass
					GRAPHICS->GetMainCameraPtr()->AddToMoveList(VEC2(1800, 1000), 0.5, 0.5f);
					// UP
					GRAPHICS->GetMainCameraPtr()->AddToMoveList(VEC2(1800, 1500), 0.5, 0.5f);
					// Alarm
					GRAPHICS->GetMainCameraPtr()->AddToMoveList(VEC2(1100, 1500), 0.5, 0.5f);
					// Bottom lane
					GRAPHICS->GetMainCameraPtr()->AddToMoveList(VEC2(1100, 700), 0.5, 0.5f);
					// Top lane
					GRAPHICS->GetMainCameraPtr()->AddToMoveList(VEC2(650, 2500), 1, 0.5f);
					// classroom hallway
					GRAPHICS->GetMainCameraPtr()->AddToMoveList(VEC2(150, 1500), 1, 0.5f);

					// Classroom 
					GRAPHICS->GetMainCameraPtr()->AddToMoveList(VEC2(50, 1900), 1, 0.5f);

					GRAPHICS->GetMainCameraPtr()->AddFollowPos(0.5f);
				}
			}

			//SOUNDMGR->PlaySoundFromMap("click");
			
		}
	
}

void TutorialScript1::TextUpdate()
{

}

void TutorialScript1::LoadResource()
{
	//FACTORY->CreateArcheType("BlackBarTop");
}

void TutorialScript1::Exit()
{
	PageCount = maxCount;
	//IntroDialogDone = false;
}