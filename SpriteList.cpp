/************************************************************************************ /
/*
File Name:		SpriteList.cpp
Project Name:	WhereGotLate
Author(s):		Tay Qin Wen Alvin [100%]

Brief:			Definition for SpriteList.


Copyright Information:
All content © 2020 DigiPen (SINGAPORE) Corporation, all rights reserved.
*/
/*************************************************************************************/
#include "stdafx.h"
#include "SpriteList.h"


SpriteList::SpriteList()
	: textureName{ "DefaultTextureName" }, sprites{}
{
}

SpriteList::~SpriteList()
{
	sprites.clear();
}

int SpriteList::GetLastFrame()
{
	return static_cast<int>(sprites.size());
}
