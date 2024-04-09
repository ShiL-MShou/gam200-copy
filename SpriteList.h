/************************************************************************************ /
/*
File Name:		SpriteList.h
Project Name:	WhereGotLate
Author(s):		Tay Qin Wen Alvin [100%]

Brief:			Declaration for SpriteList.


Copyright Information:
All content © 2020 DigiPen (SINGAPORE) Corporation, all rights reserved.
*/
/*************************************************************************************/
#pragma once

#include <vector>
#include "Vector2.h"
#include "Sprite.h"


struct SpriteList
{
	std::string textureName;
	// sprite list name
	std::string name;

	std::vector<Sprite> sprites;

	VEC2 size;

	// ---------------------------------------
	SpriteList();
	~SpriteList();

	int GetLastFrame();
};

