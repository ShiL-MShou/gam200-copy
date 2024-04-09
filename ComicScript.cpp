/************************************************************************************ /
/*
File Name:		ComicScript.cpp
Project Name:	WhereGotLate
Author(s):		Tay Qin Wen Alvin [100%]

Brief:			Handle the playing of the comic.

Copyright Information:
All content © 2020 DigiPen (SINGAPORE) Corporation, all rights reserved.
*/
/*************************************************************************************/


#include "stdafx.h"
#include "ComicScript.h"
#include "Transform.h"
#include "SpriteComponent.h"
#include "TransitionScript.h"
#include "Logic.h"
#include "FModSoundManager.h"
#include "Input.h"


ComicScript::ComicScript() 
	: currFrame{ 1 }, frameTimer{ true }, tempX{640}, transition{false}, soundsPlayed{false}
{
	frameTimer.SetTimeLeft(3.0f);
}

ComicScript::~ComicScript()
{
}

void ComicScript::Update(float _dt)
{
	UNREFERENCED_PARAMETER(_dt);

	// move until tmpX < -640
	
	//gameObjPtr.lock()->has(SpriteComponent)->SetActive(false);

	if (!soundsPlayed)
	{
		SOUNDMGR->PlaySoundLoopFromMap("comicMusic");
		SOUNDMGR->setForcePause("comicMusic", false);
		SOUNDMGR->setForcePause("menuMusic", true);
		soundsPlayed = true;
	}

	if (	!transition && 
			(	INPUTMGR->GetKeyPressed(MVK_RETURN) || INPUTMGR->GetKeyPressed(MVK_SPACE) 
				|| INPUTMGR->GetKeyPressed(MVK_LBUTTON) )
																				)
	{
		std::shared_ptr<GOC> transitionPtr = LOGIC->CreateObjectAt(VEC2(640, 360), 0, "Transition", "Transition", true);
		LOGIC->GetScript<TransitionScript>("TRANSITION_SCRIPT")->SetTransition("Stage1v2");

		transition = true;
	}

	if (spr2 == nullptr)
	{
		spr2 = LOGIC->CreateObjectAt(VEC2(640, 320), 0, "Comic", "Comic2", false)->has(SpriteComponent);
		spr2->ChangeSubTexture("COMIC_2_1");
		spr2->SetDepth(-1.74f);
	}

	if (frameTimer.IsTimeUp() == false)
	{
		frameTimer.Update();
	}
	else
	{
		// update position
		tempX -= static_cast<int>(600 * _dt);

		switch (currFrame)
		{
		case 1:
			//spr1->SetFixedPositionX(tempX);
			gameObjPtr.lock()->has(SpriteComponent)->SetFixedPositionX(static_cast<float>(tempX));
			break;

		case 2: 
			spr2->SetFixedPositionX(static_cast<float>(tempX));
			break;

		case 3: 
			spr3->SetFixedPositionX(static_cast<float>(tempX));
			break;

		case 4: 
			spr4->SetFixedPositionX(static_cast<float>(tempX));
			break;

		case 5: 
			//spr5->SetFixedPositionX(tempX);
			break;

		default:
			break;
		}

		//gameObjPtr.lock()->has(Transform)->SetPositionX(tempX);

		// change frame once current is exited
		if (tempX <= -640)
		{
			// reset
			frameTimer.SetTimeLeft(3.0f);
			tempX = 640;


			//gameObjPtr.lock()->has(SpriteComponent)->SetFixedPositionX(tempX);

			++currFrame;

			if (currFrame <= 5)
			{
				switch (currFrame)
				{
				case 2: 
					//spr2 = LOGIC->CreateObjectAt(VEC2(640, 320), 0, "Comic", "Comic2", false)->has(SpriteComponent);
					//spr2->ChangeSubTexture("COMIC_2_1");
					//spr2->SetDepth(-1.74);

					spr3 = LOGIC->CreateObjectAt(VEC2(640, 320), 0, "Comic", "Comic3", false)->has(SpriteComponent);
					spr3->ChangeSubTexture("COMIC_3_1");
					spr3->SetDepth(-1.73f);
					gameObjPtr.lock()->has(SpriteComponent)->SetActive(false);
					break;

				case 3:
					spr4 = LOGIC->CreateObjectAt(VEC2(640, 320), 0, "Comic", "Comic4", false)->has(SpriteComponent);
					spr4->ChangeSubTexture("COMIC_4_1");
					spr4->SetDepth(-1.72f);
					spr2->SetActive(false);
					break;

				case 4:
					spr5 = LOGIC->CreateObjectAt(VEC2(640, 320), 0, "Comic", "Comic5", false)->has(SpriteComponent);
					spr5->ChangeSubTexture("COMIC_5_1");
					spr5->SetDepth(-1.71f);
					spr3->SetActive(false);
					break;

				case 5:
					spr4->SetActive(false);
					break;

				default:
					break;
				}
				//gameObjPtr.lock()->has(SpriteComponent)->ChangeSubTexture("COMIC_" + std::to_string(currFrame) + "_1");
			}
			else
			{
				if (!transition)
				{
					std::shared_ptr<GOC> transitionPtr = LOGIC->CreateObjectAt(VEC2(640, 360), 0, "Transition", "Transition", true);
					LOGIC->GetScript<TransitionScript>("TRANSITION_SCRIPT")->SetTransition("Stage1v2");

					transition = true;
				}
				//SOUNDMGR->PauseAllSounds();
				//notInGame = false;
			}
		}
		else if (currFrame == 5)
		{
			if (!transition)
			{
				std::shared_ptr<GOC> transitionPtr = LOGIC->CreateObjectAt(VEC2(640, 360), 0, "Transition", "Transition", true);
				LOGIC->GetScript<TransitionScript>("TRANSITION_SCRIPT")->SetTransition("Stage1v2");

				transition = true;
			}
		}
	}
	


}

void ComicScript::Exit()
{
	if (soundsPlayed)
	{
		SOUNDMGR->setForcePause("comicMusic", true);
	}
	soundsPlayed = false;

	transition = false;
	frameTimer.SetTimeLeft(3.0f);
	currFrame = 1;
	frameTimer.SetIsActive(true);
	tempX = 640;

	//spr1->Destroy();
	if (spr2 != nullptr) 
		spr2->Destroy();
	if (spr3 != nullptr)
		spr3->Destroy();
	if (spr4 != nullptr)
		spr4->Destroy();
	if (spr5 != nullptr)
		spr5->Destroy();

	spr2 = nullptr;
	spr3 = nullptr;
	spr4 = nullptr;
	spr5 = nullptr;

}

void ComicScript::LoadResource()
{
	FACTORY->CreateArcheType("Comic");
}
