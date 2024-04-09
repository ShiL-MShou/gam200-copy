/*************************************************************************************/
/*
File Name:		ScriptCheckBoxMute.h
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

class ScriptCheckBoxMute : public Script
{
	//std::weak_ptr<GOC>				gameObjPtr;
	std::shared_ptr<BoxCollider>		colliderPtr;
	std::shared_ptr<Transform>			transformPtr;
	std::shared_ptr<SpriteComponent>	spritePtr;

	bool isMute;

public:
	ScriptCheckBoxMute() : isMute{ false } {}
	~ScriptCheckBoxMute();

	virtual void Start() override;
	virtual void Update(float _dt) override;
	virtual void Exit() override;

	virtual void SetOwner(std::shared_ptr<GOC> _gameObjPtr) override;

	//virtual void OnClick();

	virtual void OnRelease();
	virtual void OnHold();

	void LoadResource() override;
};

