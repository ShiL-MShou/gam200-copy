/************************************************************************************ /
/*
File Name:		AnimationComponent.h
Project Name:	WhereGotLate
Author(s):		Tay Qin Wen Alvin [100%]


Brief:			Animation component.


Copyright Information:
All content © 2019 DigiPen (SINGAPORE) Corporation, all rights reserved.
*/
/*************************************************************************************/

#pragma once

#include <vector>
#include "Component.h"



class AnimationComponent :	public Component
{

public:
	std::vector<int> animationList;
	int currAnimation;
	int startingAnimation;

	int currFrame;
	float currFrameTime;
	bool repeat;

	AnimationComponent();
	~AnimationComponent();

	// delete copy
	AnimationComponent(const AnimationComponent&) = delete;
	AnimationComponent& operator=(const AnimationComponent&) = delete;


	void Initialize() override;
	void Inspect() override;

	std::shared_ptr<Component> Clone() override;
	void Destroy() override;

	void Serialize(JsonReader& _jr) override;

	void DeSerialize(JsonWriter& _jw) override;
};

