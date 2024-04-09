/************************************************************************************ /
/*
File Name:		Animation.h
Project Name:	WhereGotLate
Author(s):		Tay Qin Wen Alvin [100%]


Brief:			Animation holds all the information of its frame.


Copyright Information:
All content © 2019 DigiPen (SINGAPORE) Corporation, all rights reserved.
*/
/*************************************************************************************/


#pragma once

#include <vector>
#include <string>

#include "Frame.h"
#include "AnimationComponent.h"
#include "Composition.h"


struct Animation
{
	//int textureID;

	std::string textureName;
	// animation name
	std::string name;

	std::vector<Frame> frames;
	
	std::weak_ptr<AnimationComponent> animationCompPtr;

	// KIV
	int startFrame;
	int lastFrame;

	VEC2 size;


	Animation();
	~Animation();

	void SetOwner(std::shared_ptr<GOC> _owner);
	void Update(float _dt);
};

