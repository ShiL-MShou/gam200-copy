/************************************************************************************ /
/*
File Name:		AnimationManager.h
Project Name:	WhereGotLate
Author(s):		Tay Qin Wen Alvin [100%]


Brief:			This holds all the animation component and manages them.


Copyright Information:
All content © 2019 DigiPen (SINGAPORE) Corporation, all rights reserved.
*/
/*************************************************************************************/


#pragma once

#include <vector>
#include <memory>
#include "AnimationComponent.h"
#include "Singleton.h"

#define MyAnimationManager (AnimationManager::GetInstance())


class AnimationManager : public Singleton<AnimationManager>
{
	std::vector<std::shared_ptr<AnimationComponent>> animationCompList;

	AnimationManager();
	~AnimationManager();

	AnimationManager(const AnimationManager&) = delete;
	AnimationManager& operator=(const AnimationManager&) = delete;

	friend class Singleton<AnimationManager>;

public:
	void Update(float _dt);

	void Initialize();

	void Clear();

	void AddAnimationComponent(std::shared_ptr<AnimationComponent> _animationCompPtr);

	bool IsLastFrame(std::shared_ptr<AnimationComponent> _animationCompPtr);

	void RemoveComponent(std::shared_ptr<AnimationComponent> _compPtr);
};
