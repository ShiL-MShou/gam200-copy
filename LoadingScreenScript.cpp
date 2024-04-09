/************************************************************************************ /
/*
File Name:		LoadingScreenScript.cpp
Project Name:	WhereGotLate
Author(s):		Tay Qin Wen Alvin [100%]

Brief:			Handle the playing of the loading screen,
				digipen logo, team logo and loading of the textures.

Copyright Information:
All content © 2020 DigiPen (SINGAPORE) Corporation, all rights reserved.
*/
/*************************************************************************************/

#include "stdafx.h"
#include "LoadingScreenScript.h"

#include "Graphics.h"
#include "Logic.h"
#include "CoreEngine.h"
#include "TransitionScript.h"


LoadingScreenScript::LoadingScreenScript()
	:	loadCounter{ 0 }, jr{ nullptr }, jKey{ "Texture" }, digipenLogoTimer{ true }, teamLogoTimer{ false }, loadingTimer{ false },
	isLoadComplete{ false }, startX{ 100 }, endX{ 1150 }, maxCounter{ 100 }, transition{ false }
{
	jr = std::make_shared<JsonReader>(CORE->mPath + "Graphics/textures.json");

	digipenLogoTimer.SetTimeLeft(2.0f);
	teamLogoTimer.SetTimeLeft(1.5f);
	loadingTimer.SetTimeLeft(1.0f);

}

LoadingScreenScript::~LoadingScreenScript()
{
}


void LoadingScreenScript::Start()
{
}

void LoadingScreenScript::Init()
{
}

void LoadingScreenScript::Update(float _dt)
{
	UNREFERENCED_PARAMETER(_dt);

	// loading is unsuccessful / has finished loading
	JsonReader tmpJr(CORE->mPath + "Graphics/textures.json");

	bool isLoadSuccess = GRAPHICS->LoadAssets(tmpJr, jKey, loadCounter);

	if (!digipenLogoTimer.IsTimeUp())
	{
		digipenLogoTimer.Update();

		if (digipenLogoTimer.IsTimeUp())
		{
			gameObjPtr.lock()->has(SpriteComponent)->ChangeSubTexture("TEAM_LOGO_1");
			gameObjPtr.lock()->has(Transform)->SetScale(VEC2(459.0f, 459.0f));
			teamLogoTimer.SetIsActive(true);
		}
	}
	else if (!teamLogoTimer.IsTimeUp())
	{
		teamLogoTimer.Update();

		if (teamLogoTimer.IsTimeUp())
		{
			gameObjPtr.lock()->has(SpriteComponent)->ChangeSubTexture("LOADING_BG_1");
			gameObjPtr.lock()->has(Transform)->SetScale(VEC2(1280.0f, 720.0f));
			loadingTimer.SetIsActive(true);
		}

	}
	else if (!loadingTimer.IsTimeUp() || isLoadComplete == false)
	{
		loadingTimer.Update();

		if (runningBoy == nullptr)
			runningBoy = LOGIC->CreateObjectAt(VEC2((float)startX, 140.0f), 0, "RunningBoy", "RunningBoy", false);
		else
			runningBoy->has(Transform)->SetPositionX(startX + (endX - startX) * loadCounter / (float)maxCounter * (1.0f - loadingTimer.GetTimeLeft()));

		if (loadCounter >= maxCounter)
			isLoadComplete = true;
	}
	else if (loadCounter != 0 && isLoadSuccess == false)
	{
		//LOGIC->ChangeLevel("MainMenu");
		if (transition == false)
		{
			std::shared_ptr<GOC> transitionPtr = LOGIC->CreateObjectAt(VEC2(640, 360), 0, "Transition", "Transition", true);
			//TransitionScript ts = transitionPtr->has(LogicComponent)->GetScriptName

			LOGIC->GetScript<TransitionScript>("TRANSITION_SCRIPT")->SetTransition("MainMenu");

			transition = true;
		}
	}


	if (loadCounter < maxCounter)
		++loadCounter;
}

void LoadingScreenScript::Exit()
{
}

void LoadingScreenScript::LoadResource()
{
	FACTORY->CreateArcheType("RunningBoy");
	FACTORY->CreateArcheType("Transition");
}
