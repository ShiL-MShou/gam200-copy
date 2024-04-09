/************************************************************************************ /
/*
File Name:		ScriptButton.h
Project Name:	WhereGotLate
Author(s):		Tay Qin Wen Alvin [100%]


Brief:			This is the base of all the script button.


Copyright Information:
All content © 2019 DigiPen (SINGAPORE) Corporation, all rights reserved.
*/
/*************************************************************************************/

#pragma once

#include "Script.h"

#include <memory>
#include "Composition.h"
#include "Transform.h"
#include "BoxCollider.h"
#include "SpriteComponent.h"


class ScriptButton : public Script
{
	std::shared_ptr<BoxCollider>	colliderPtr;
	std::shared_ptr<Transform>		transformPtr;
	std::shared_ptr<SpriteComponent>			spritePtr;


public:
	ScriptButton();
	~ScriptButton();

	virtual void Start() override;
	virtual void Update(float _dt) override;
	virtual void Exit() override;
	
	virtual void SetOwner(std::shared_ptr<GOC> _gameObjPtr) override;

	virtual void CollisionResponse();

	virtual void OnRelease();
	virtual void OnHold();

	void LoadResource() override;

	std::weak_ptr<GOC>				selectedPtr;
};
