/************************************************************************************ /
/*
File Name:		Sprite.cpp
Project Name:	WhereGotLate
Author(s):		Tay Qin Wen Alvin [100%]

Brief:			Definition for Sprite.


Copyright Information:
All content © 2020 DigiPen (SINGAPORE) Corporation, all rights reserved.
*/
/*************************************************************************************/

#include "stdafx.h"
#include "Sprite.h"


Sprite::Sprite()
	: uv{}
{
}

Sprite::Sprite(std::array<VEC2, 4> _arr)
{
	std::copy(std::begin(_arr), std::end(_arr), std::begin(uv));
}

Sprite::~Sprite()
{
}
