#include "stdafx.h"
#include "ResultScript.h"
#include "Factory.h"
#include "Logic.h"
#include "Input.h"
#include "FModSoundManager.h"

ResultScript::ResultScript() :  score{0}, 
								playSound{false}, 
								bgFLoaded{false}, 
								picLoaded{false}
{
}

ResultScript::~ResultScript()
{
}

void ResultScript::Start()
{
}

void ResultScript::Init()
{
	//score = MyScore.getTotalNotesCollected() + (2 * MyScore.getTotalStageClears());
	//std::cout << "End Score: " << score << std::endl;
	//if(score < 3)
	//	backgroundPtr = LOGIC->CreateObjectAt(VEC2(0, 0), 0, "BackgroundF", "bgF", false)->has(SpriteComponent);
	//else if(score > 3 && score < 8)
	//	backgroundPtr = LOGIC->CreateObjectAt(VEC2(0, 0), 0, "BackgroundC", "bgC", false)->has(SpriteComponent);
	//else
	//	backgroundPtr = LOGIC->CreateObjectAt(VEC2(0, 0), 0, "BackgroundF", "bgF", false)->has(SpriteComponent);
}

void ResultScript::Update(float _dt)
{
	UNREFERENCED_PARAMETER(_dt);
	MyScore.displayTotalStats();
	MyEvent.inGameStage = false;

	if (!picLoaded)
	{
		score = MyScore.getTotalNotesCollected() + (2 * MyScore.getTotalStageClears());
		std::cout << "End Score: " << score << std::endl;
		picLoaded = true;
	}

	if(!playSound)
		SOUNDMGR->setForcePause("BGM", true);

	if (score <= 4)
	{
		backgroundAPtr.lock()->SetActive(false);
		backgroundCPtr.lock()->SetActive(false);
		backgroundFPtr.lock()->SetActive(true);

		gradeAPtr.lock()->SetActive(false);
		gradeCPtr.lock()->SetActive(false);
		gradeFPtr.lock()->SetActive(true);
		if (!playSound)
		{
			SOUNDMGR->setForcePause("ASound", true);
			SOUNDMGR->setForcePause("CSound", true);
			SOUNDMGR->setForcePause("FSound", false);
			SOUNDMGR->PlaySoundFromMap("FSound");

			playSound = true;
		}

	}
	else if (score <= 10 && score > 4)
	{
		backgroundAPtr.lock()->SetActive(false);
		backgroundCPtr.lock()->SetActive(true);
		backgroundFPtr.lock()->SetActive(false);

		gradeAPtr.lock()->SetActive(false);
		gradeFPtr.lock()->SetActive(false);
		gradeCPtr.lock()->SetActive(true);

		if (!playSound)
		{
			SOUNDMGR->setForcePause("ASound", true);
			SOUNDMGR->setForcePause("FSound", true);
			SOUNDMGR->setForcePause("CSound", false);
			SOUNDMGR->PlaySoundFromMap("CSound");

			playSound = true;
		}
	}
	else if (score > 10)
	{
		backgroundCPtr.lock()->SetActive(false);
		backgroundFPtr.lock()->SetActive(false);
		backgroundAPtr.lock()->SetActive(true);
		
		gradeCPtr.lock()->SetActive(false);
		gradeFPtr.lock()->SetActive(false);
		gradeAPtr.lock()->SetActive(true);

		if (!playSound)
		{
			SOUNDMGR->setForcePause("FSound", true);
			SOUNDMGR->setForcePause("CSound", true);
			SOUNDMGR->setForcePause("ASound", false);
			SOUNDMGR->PlaySoundFromMap("ASound");

			playSound = true;
		}
	}
		

	if (INPUTMGR->GetKeyPressed(MVK_J))
	{
		score = 0;
		playSound = false;
		SOUNDMGR->PauseAllSounds();
	}
	if (INPUTMGR->GetKeyPressed(MVK_K))
	{
		score = 5;
		playSound = false;
		SOUNDMGR->PauseAllSounds();

	}
	if (INPUTMGR->GetKeyPressed(MVK_L))
	{
		score = 9;
		playSound = false;
		SOUNDMGR->PauseAllSounds();

	}
}

void ResultScript::Exit()
{
	//backgroundPtr = nullptr;
	if (playSound)
	{
		SOUNDMGR->setForcePause("ASound", true);
		SOUNDMGR->setForcePause("CSound", true);
		SOUNDMGR->setForcePause("FSound", true);
	}
	picLoaded = false;
	playSound = false;
	bgFLoaded = false;

}

void ResultScript::LoadResource()
{
	FACTORY->CreateArcheType("BackgroundC");
	FACTORY->CreateArcheType("BackgroundF");
	FACTORY->CreateArcheType("BackgroundA");
	FACTORY->CreateArcheType("GradeA");
	FACTORY->CreateArcheType("GradeC");
	FACTORY->CreateArcheType("GradeF");

	backgroundCPtr = LOGIC->CreateObjectAt(VEC2(0, 0), 0, "BackgroundC", "bgC", false)->has(SpriteComponent);
	backgroundFPtr = LOGIC->CreateObjectAt(VEC2(0, 0), 0, "BackgroundF", "bgF", false)->has(SpriteComponent);
	backgroundAPtr = LOGIC->CreateObjectAt(VEC2(0, 0), 0, "BackgroundA", "bgF", false)->has(SpriteComponent);

	backgroundCPtr.lock()->SetActive(false);
	backgroundFPtr.lock()->SetActive(false);
	backgroundAPtr.lock()->SetActive(false);
	
	gradeAPtr = LOGIC->CreateObjectAt(VEC2(0, 0), 0, "GradeA", "gA", false)->has(SpriteComponent);
	gradeCPtr = LOGIC->CreateObjectAt(VEC2(0, 0), 0, "GradeC", "gC", false)->has(SpriteComponent);
	gradeFPtr = LOGIC->CreateObjectAt(VEC2(0, 0), 0, "GradeF", "gF", false)->has(SpriteComponent);

	gradeAPtr.lock()->SetActive(false);
	gradeCPtr.lock()->SetActive(false);
	gradeFPtr.lock()->SetActive(false);
}
