/*************************************************************************************/
/*
File Name:		SpriteManager.cpp
Project Name:	WhereGotLate
Author(s):		Tay Qin Wen Alvin [100%]


Brief:		SpriteManager store and maintains the list of Sprites


Copyright Information:
All content © 2019 DigiPen (SINGAPORE) Corporation, all rights reserved.
*/
/*************************************************************************************/


#include "stdafx.h"
#include "SpriteManager.h"
#include "Graphics.h"


SpriteManager::SpriteManager()
	: counter{ 1 }
{
}

SpriteManager::~SpriteManager()
{
}

void SpriteManager::Initialize()
{
}

int SpriteManager::AddSprite(std::shared_ptr<SpriteComponent> _spr)
{
	spriteList.insert(std::make_pair( _spr, counter));
	_spr->SetID(counter);

	return counter++;
}

void SpriteManager::RemoveSprite(int _sprID)
{
	//spriteList.erase(_sprID);

	for (auto itr = std::begin(spriteList); itr != std::end(spriteList); ++itr)
	{
		if (itr->second == _sprID)
		{
			spriteList.erase(itr);
			return;
		}
	}

	//assert(0 && "Sprite not found!");
}
//
//int SpriteManager::AddFixedSprite(std::shared_ptr<Sprite> _spr)
//{
//	fixedSpriteList.insert(std::make_pair( _spr, counter ));
//	_spr->SetID(counter);
//
//	return counter++;
//}
//
//void SpriteManager::RemoveFixedSprite(int _sprID)
//{
//	//fixedSpriteList.erase(_sprID);
//
//	for (auto itr = std::begin(fixedSpriteList); itr != std::end(fixedSpriteList); ++itr)
//	{
//		if (itr->second == _sprID)
//		{
//			fixedSpriteList.erase(itr);
//			return;
//		}
//			
//	}
//
//	//assert(0 && "Sprite not found!");
//}

void SpriteManager::normalToFixed(int _sprID)
{
	for (auto itr = std::begin(spriteList); itr != std::end(spriteList); ++itr)
	{
		if (itr->second == _sprID)
		{
			itr->first->SetMovementType(SpriteMovementType::FIXED);

			return;
		}
	}

	assert(0 && "Sprite not found!");


}

void SpriteManager::FixedToNormal(int _sprID)
{
	for (auto itr = std::begin(spriteList); itr != std::end(spriteList); ++itr)
	{
		if (itr->second == _sprID)
		{
			itr->first->SetMovementType(SpriteMovementType::FREE);

			return;
		}
	}

	assert(0 && "Sprite not found!");
}

bool SpritePtrComp::operator()(std::shared_ptr<SpriteComponent> a, std::shared_ptr<SpriteComponent> b) const
{
	// compare depth
	if (a->GetDepth() != b->GetDepth())
		return a->GetDepth() > b->GetDepth();

	else if (a->GetType() != b->GetType())
		return a->GetType() < b->GetType();

	// compare for SINGLE
	else if (a->GetType() == SpriteType::SINGLE && b->GetType() == SpriteType::SINGLE)
	{
		if (a->GetCurrTextureName() != b->GetCurrTextureName())
			return a->GetCurrTextureName() < b->GetCurrTextureName();
		else 
			return a->GetCurrSubTextureName() < b->GetCurrSubTextureName();
	}

	// compare for ANIMATED
	else 
	{
		std::shared_ptr<AnimationComponent> animationCompPtrA = a->GetBase()->has(AnimationComponent);
		std::shared_ptr<AnimationComponent> animationCompPtrB = b->GetBase()->has(AnimationComponent);

		if ((animationCompPtrA->animationList.at(animationCompPtrA->currAnimation)) !=
			(animationCompPtrB->animationList.at(animationCompPtrB->currAnimation)))
		{
			return animationCompPtrA->currFrame < animationCompPtrB->currFrame;
		}
		else
			return	(animationCompPtrA->animationList.at(animationCompPtrA->currAnimation)) < 
					(animationCompPtrB->animationList.at(animationCompPtrB->currAnimation));
	}


		//(a->GetCurrTextureName() < b->GetCurrTextureName());// || (a->GetCurrTextureName() == b->GetCurrTextureName());
	
}
