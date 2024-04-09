/*************************************************************************************/
/*
File Name:		WinScript.cpp
Project Name:	WhereGotLate
Author(s):		Ong Cheng Kai Marcus [90%]
				Tay Qin Wen Alvin 	 [10%]

Copyright Information:
All content copyright 2020 DigiPen (SINGAPORE) Corporation, all rights reserved.
*/
/*************************************************************************************/
#include "stdafx.h"
#include "WinScript.h"
#include "CoreEngine.h"
#include "FModSoundManager.h"
#include "TextScript.h"
#include "Logic.h"
#include "Input.h"
#include "Graphics.h"
#include "PlayerScript.h" //inDialog
#include "Score.h"


bool inWinDialog;


WinScript::WinScript()
	: isParticlesInit{ false },
	  WinDialog{false},
	  ShowScore{false},
	  playedSound{false}
{
}

WinScript::~WinScript()
{
}

void WinScript::Start()
{
}

void WinScript::Init()
{
	isParticlesInit = false;
	WinDialog = false;
	inWinDialog = false;
	reportCardPtr = LOGIC->CreateObjectAt(VEC2(0, 0), 0, "ReportCardBlank", "Text", false)->has(SpriteComponent);
	reportCardPtr.lock()->SetActive(false);
	
	sweePtr = LOGIC->CreateObjectAt(VEC2(0, 0), 0, "Win", "Lel", false)->has(SpriteComponent);
	sweePtr.lock()->SetActive(false);
}

void WinScript::Update(float _dt)
{
	MyEvent.inGameStage = false;
	if (!playedSound)
	{
		SOUNDMGR->PauseSoundFromMap("BGM");
		SOUNDMGR->setForcePause("BGM", true);
		SOUNDMGR->PlaySoundFromMap("winSound");
		SOUNDMGR->PlaySoundLoopFromMap("winMusic");
		SOUNDMGR->setForcePause("winMusic", false);
		playedSound = true;
	}

	if (!WinDialog)
	{
		//CORE->SetPausedStatus(true);
		inDialog = true;
		MyEvent.textPtr.lock()->SetActive(true);
	    MyEvent.boyPtr.lock()->SetActive(true);
		MyEvent.tintPtr.lock()->SetActive(true);

		//sound
		SOUNDMGR->PlaySoundFromMap("boyVoice");
		
		if(strcmp(CORE->GetCurrLevel().c_str(), "Stage1v2") == 0)
			MyEvent.textscriptPtr.lock()->Serialize(CORE->mPath + "Text/BoyComments/WinSpeech.json");
		else if(strcmp(CORE->GetCurrLevel().c_str(), "Stage2v2") == 0)
			MyEvent.textscriptPtr.lock()->Serialize(CORE->mPath + "Text/BoyComments/WinSpeech2.json");
		else if(strcmp(CORE->GetCurrLevel().c_str(), "Stage3v2") == 0)
			MyEvent.textscriptPtr.lock()->Serialize(CORE->mPath + "Text/BoyComments/WinSpeech3.json");
		else if(strcmp(CORE->GetCurrLevel().c_str(), "Stage4") == 0)
			MyEvent.textscriptPtr.lock()->Serialize(CORE->mPath + "Text/BoyComments/WinSpeech4.json");

		WinDialog = true;
		inWinDialog = true;

		MyScore.incrementStages();
	}

	if (inWinDialog && (INPUTMGR->GetKeyPressed(MVK_RETURN) || INPUTMGR->GetKeyPressed(MVK_SPACE) || INPUTMGR->GetKeyPressed(MVK_LBUTTON))) 
	{
		//CORE->SetPausedStatus(false);
		/*if (std::shared_ptr<GOC> text = textObjPtr.lock())
			text->Destroy();
		if (std::shared_ptr<GOC> boySpr = boySpritePtr.lock())
			boySpr->Destroy();
		if (std::shared_ptr<GOC> tint = tintPtr.lock())
			tint->Destroy();*/
		MyEvent.textPtr.lock()->SetActive(false);
		MyEvent.boyPtr.lock()->SetActive(false);
		MyEvent.tintPtr.lock()->SetActive(false);
		inWinDialog = false;
		reportCardPtr.lock()->SetActive(true);
		sweePtr.lock()->SetActive(true);
		ShowScore = true;
	}

	if(ShowScore)
		MyScore.displayStageCleared();


	if (!isParticlesInit)
	{
		isParticlesInit = true;
		VEC2 cameraOffset = GRAPHICS->GetMainCameraPtr()->GetPosition();
		srand(static_cast <unsigned> (time(0)));
		for (int i = 0; i < 15; ++i)
		{
			float emitterLifetime = 1000000.0f;
			float particleLifetime = 15.0f;
			float emitInterval = 1.5f + static_cast<float>(rand()) / static_cast<float>(RAND_MAX / (5.0f - 1.5f));
			float offsetX = WinWidth / 15.0f * i + 30.0f;
			float startDelay = static_cast<float>(rand()) / static_cast<float>(RAND_MAX / (3.0f));
			GRAPHICS->CreateEmitter(EmitterType::STATIC_RANDOM, VEC2(-cameraOffset.x + offsetX, -cameraOffset.y + 720.0f), startDelay, emitterLifetime, particleLifetime, 0.0f, emitInterval, 1.0f, VEC2(2.0f, 2.0f), 0, 5.0f, "Particles");
		}

		//if (scoreNum == 50)
		//{
		//	gradePtr = LOGIC->CreateObjectAt(VEC2(0, 0), 20, "Grades", "C");
		//	gradePtr.lock()->has(SpriteComponent)->SetCurrFrame(2);
		//}
		//else if ( scoreNum == 75)
		//{
		//	gradePtr = LOGIC->CreateObjectAt(VEC2(0, 0), 30, "Grades", "B");
		//	gradePtr.lock()->has(SpriteComponent)->SetCurrFrame(1);
		//}
		//else if (scoreNum == 100)
		//{
		//	gradePtr = LOGIC->CreateObjectAt(VEC2(0, 0), 20, "Grades", "A");
		//	gradePtr.lock()->has(SpriteComponent)->SetCurrFrame(0);
		//}
	}
	UNREFERENCED_PARAMETER(_dt);
	//if (INPUTMGR->GetKeyPressed(MVK_RETURN))
	//{
	//	scoreNum = 0;
	//	LOGIC->ChangeLevel("MainMenu");
	//}
}
void WinScript::Exit()
{
	WinDialog = false;
	inWinDialog = false;
	ShowScore = false;
	if (std::shared_ptr<GOC> grade = gradePtr.lock())
		grade->Destroy();
	if(playedSound)
		SOUNDMGR->setForcePause("winMusic", true);
	playedSound = false;
}

void WinScript::LoadResource()
{
	FACTORY->CreateArcheType("ReportCardBlank");
	FACTORY->CreateArcheType("Win");
}
