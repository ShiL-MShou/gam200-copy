/************************************************************************************ /
/*
File Name:		LoadingScreenScript.h
Project Name:	WhereGotLate
Author(s):		Tay Qin Wen Alvin [100%]

Brief:			Handle the playing of the loading screen,
				digipen logo, team logo and loading of the textures.

Copyright Information:
All content © 2020 DigiPen (SINGAPORE) Corporation, all rights reserved.
*/
/*************************************************************************************/

#pragma once

#include "Script.h"
#include "Graphics.h"

#include "Timer.h"


class LoadingScreenScript : public Script
{
private:
	int loadCounter;
	std::shared_ptr<JsonReader> jr;
	std::string jKey;

	Timer digipenLogoTimer;
	Timer teamLogoTimer;
	Timer loadingTimer;

	int startX;
	int endX;

	int maxCounter;
	bool isLoadComplete;

	bool transition;

	std::shared_ptr<GOC> runningBoy;


public:
	LoadingScreenScript();
	~LoadingScreenScript();

	LoadingScreenScript(const LoadingScreenScript&) = delete;
	LoadingScreenScript& operator=(const LoadingScreenScript&) = delete;

	virtual void Start() override;
	virtual void Init() override;
	virtual void Update(float _dt) override;
	virtual void Exit() override;

	virtual void LoadResource() override;
};