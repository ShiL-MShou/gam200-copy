/*************************************************************************************/
/*
File Name:		SpriteManager.h
Project Name:	WhereGotLate
Author(s):		Tay Qin Wen Alvin [100%]


Brief:		SpriteManager store and maintains the list of Sprites


Copyright Information:
All content © 2019 DigiPen (SINGAPORE) Corporation, all rights reserved.
*/
/*************************************************************************************/



#pragma once

#include <set>
#include "SpriteComponent.h"
#include "Singleton.h"

#define MySpriteManager (SpriteManager::GetInstance())


struct SpritePtrComp
{
	// sorting for the sprites
	bool operator()(std::shared_ptr<SpriteComponent> a, std::shared_ptr<SpriteComponent> b) const;
};


class SpriteManager : public Singleton<SpriteManager>
{
	int counter;

	std::multimap<std::shared_ptr<SpriteComponent>, int, SpritePtrComp> spriteList;
	
	SpriteManager();
	~SpriteManager();

	SpriteManager(const SpriteManager&) = delete;
	SpriteManager& operator=(const SpriteManager&) = delete;

	friend class Singleton<SpriteManager>;

public:
	void Initialize();

	int AddSprite(std::shared_ptr<SpriteComponent> _spr);
	void RemoveSprite(int _sprID);

	void normalToFixed(int _sprID);
	void FixedToNormal(int _sprID);

	friend class Graphics;
};


