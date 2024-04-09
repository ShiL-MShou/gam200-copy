/************************************************************************************ /
/*
File Name:		Sprite.h
Project Name:	WhereGotLate
Author(s):		Tay Qin Wen Alvin [100%]

Brief:			Declaration for Sprite.


Copyright Information:
All content © 2020 DigiPen (SINGAPORE) Corporation, all rights reserved.
*/
/*************************************************************************************/
#pragma once

#include <array>
#include "Vector2.h"


struct Sprite
{
	std::array<VEC2, 4> uv;

	Sprite();
	Sprite(std::array<VEC2, 4> _arr);
	~Sprite();
};

