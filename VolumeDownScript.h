/*************************************************************************************/
/*
File Name:		VolumeDownScript.h
Project Name:	WhereGotLate
Author(s):		Kok Zhe Yan			 [100%]


Copyright Information:
All content © 2019 DigiPen (SINGAPORE) Corporation, all rights reserved.
*/
/*************************************************************************************/
#pragma once
#include "Script.h"

#include "Transform.h"
#include "BoxCollider.h"
#include "SpriteComponent.h"

class VolumeDownScript : public Script
{
	//std::weak_ptr<GOC>			gameObjPtr;
	std::shared_ptr<BoxCollider>	colliderPtr;
	std::shared_ptr<Transform>		transformPtr;
	std::shared_ptr<SpriteComponent>			spritePtr;

public:
	VolumeDownScript() {}
	~VolumeDownScript();

	virtual void Start() override;
	virtual void Update(float _dt) override;
	virtual void Exit() override;

	virtual void SetOwner(std::shared_ptr<GOC> _gameObjPtr) override;

	//virtual void OnClick();

	virtual void OnRelease();
	virtual void OnHold();

	void LoadResource() override;
};
