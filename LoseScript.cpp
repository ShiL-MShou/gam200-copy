/************************************************************************************ /
/*
File Name:		LoseScript.cpp
Project Name:	WhereGotLate
Author(s):		Tay Qin Wen Alvin [100%]


Brief:			Script for lose condition.

Copyright Information:
All content © 2019 DigiPen (SINGAPORE) Corporation, all rights reserved.
*/
/*************************************************************************************/

#include "stdafx.h"
#include "LoseScript.h"
#include "FModSoundManager.h"
#include "Logic.h"
#include "Input.h"
#include "PlayerScript.h" //inDialog
#include "Score.h" //myScore

LoseScript::LoseScript()
	: isParticlesInit{ false }, soundsPlayed{false}
{
}

LoseScript::~LoseScript()
{
}

void LoseScript::Start()
{
	//SOUNDMGR->PauseAllSounds();
	//if (!(strcmp("Stage1v2", CORE->GetCurrLevel().c_str()) == 0))
	//	SOUNDMGR->PlaySoundLoopFromMap("Nag");
	//SOUNDMGR->setForcePause("Nag", false);
	//SOUNDMGR->PlaySoundFromMap("loseSound");
}

void LoseScript::Update(float _dt)
{
	MyScore.displayStageCleared();

	MyEvent.inGameStage = false;

	if (soundsPlayed == false)
	{
		SOUNDMGR->PauseAllSounds();
		if (!(strcmp("Stage1v2", CORE->GetCurrLevel().c_str()) == 0))
		{
			SOUNDMGR->PlaySoundLoopFromMap("Nag");
			SOUNDMGR->setForcePause("Nag", false);
		}
		SOUNDMGR->PlaySoundFromMap("loseSound");
		SOUNDMGR->setForcePause("BGM", true);
		soundsPlayed = true;
	}

	if (!isParticlesInit)
	{
		isParticlesInit = true;

		VEC2 cameraOffset = GRAPHICS->GetMainCameraPtr()->GetPosition();

		srand(static_cast <unsigned> (time(0)));

		/*gradePtr = LOGIC->CreateObjectAt(VEC2(0, 0), 30.0f, "Grades", "F");
		gradePtr.lock()->has(SpriteComponent)->SetFixedPosition(VEC2{ 470.0f, 250.0f });
		gradePtr.lock()->has(SpriteComponent)->SetCurrFrame(5);*/

		inDialog = true;


		if (strcmp("Stage2v2", CORE->GetCurrLevel().c_str()) == 0)
		{
			npcPtr = LOGIC->CreateObjectAt(VEC2(0, 0), 0, "TeacherScold", "teacherscold");
			npcPtr.lock()->has(SpriteComponent)->SetFixedPosition(VEC2{ 1020.0f, 300.0f });
			npcPtr = LOGIC->CreateObjectAt(VEC2(0, 0), 0, "PrefectScold", "prefectscold");
			npcPtr.lock()->has(SpriteComponent)->SetFixedPosition(VEC2{ 1150.0f, 250.0f });
		}
		else if (strcmp("Stage1v2", CORE->GetCurrLevel().c_str()) == 0 || strcmp("Stage3v2", CORE->GetCurrLevel().c_str()) == 0)
		{
			npcPtr = LOGIC->CreateObjectAt(VEC2(0, 0), 0, "PrefectScold", "prefectscold");
			npcPtr.lock()->has(SpriteComponent)->SetFixedPosition(VEC2{ 1000.0f, 250.0f });
		}
		

		for (int i = 0; i < 1; ++i)
		{

			float emitterLifetime = 1000000.0f;
			float particleLifetime = 15.0f;

			float emitInterval = 1.5f + static_cast<float>(rand()) / static_cast<float>(RAND_MAX / (5.0f - 1.5f));
			float offsetX = 790.0f;

			float startDelay = static_cast<float>(rand()) / static_cast<float>(RAND_MAX / (3.0f));

			GRAPHICS->CreateEmitter(EmitterType::STATIC, VEC2(-cameraOffset.x + offsetX, -cameraOffset.y + 255.0f), startDelay, emitterLifetime, particleLifetime, 0.0f, emitInterval, 1.0f, VEC2(2.0f, 2.0f), 0, 5.0f, "SweatDrops");


		}

	}

	UNREFERENCED_PARAMETER(_dt);
	if (INPUTMGR->GetKeyPressed(MVK_RETURN) || INPUTMGR->GetKeyPressed(MVK_LBUTTON))
	{
		
		//LOGIC->ChangeLevel("MainMenu");
	}
}

void LoseScript::Exit()
{
	if(std::shared_ptr<GOC> grade = gradePtr.lock())
		grade->Destroy();
	isParticlesInit = false;
	if (soundsPlayed)
	{
		SOUNDMGR->PauseSoundFromMap("Nag");
		SOUNDMGR->setForcePause("Nag", true);
	}
	soundsPlayed = false;

}

void LoseScript::LoadResource()
{
	FACTORY->CreateArcheType("Grades");
	FACTORY->CreateArcheType("TeacherScold");
	FACTORY->CreateArcheType("PrefectScold");
}
