/************************************************************************************ /
/*
File Name:		ComicScript.h
Project Name:	WhereGotLate
Author(s):		Tay Qin Wen Alvin [100%]

Brief:			Handle the playing of the comic.

Copyright Information:
All content © 2020 DigiPen (SINGAPORE) Corporation, all rights reserved.
*/
/*************************************************************************************/

#pragma once
#include "Script.h"
#include "Timer.h"
#include <memory>
#include "SpriteComponent.h"


class ComicScript : public Script
{
	int currFrame;
	Timer frameTimer;
	int tempX;

	// comicScript will hold the first frame
	//std::shared_ptr<SpriteComponent> spr1;
	std::shared_ptr<SpriteComponent> spr2;
	std::shared_ptr<SpriteComponent> spr3;
	std::shared_ptr<SpriteComponent> spr4;
	std::shared_ptr<SpriteComponent> spr5;

	bool transition;

	bool soundsPlayed;

public: 
	ComicScript();
	~ComicScript();

	void Update(float _dt) override;

	void LoadResource() override;

	void Exit() override;
};

