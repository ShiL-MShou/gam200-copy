/******************************************************************************/
/*
	File Name: NPCScript.h
	Project Name: WhereGotLate
	Author(s): Lee Shi Lin [100%]

	Brief: Set for Teacher NPC behaviour

	Copyright Information:
	All content © 2019 DigiPen (SINGAPORE) Corporation, all rights reserved.
*/
/******************************************************************************/
#pragma once
#include "stdafx.h"
#include <vector>
#include <utility>
#include <string>
#include "Script.h"
#include "StateMachine.h"
#include "List_StateLinker.h"
#include "PATROL_STATE.h"
#include "IDLE_STATE.h"
#include "DISTRACTED_STATE.h"
#include "CHASE_STATE.h"
#include "Logic.h"
#include "Body.h"
#include "StateEnum.h"


class NPCScript : public Script
{
protected:
	bool isRight;
	bool isUp;

	int dir;

	std::shared_ptr<Body>	bodyPtr;
	std::shared_ptr<SpriteComponent> spritePtr;

	std::map<std::string, bool> alerted;

	//bool alerted = false;
	bool isShadowCreated;
	bool isStunned;

public:
	NPCScript();
	~NPCScript();

	NPCScript(const NPCScript&) = delete;
	NPCScript& operator=(const NPCScript&) = delete;

	std::shared_ptr<StateMachine> sm;// = std::make_shared<StateMachine>();

	virtual void Start() override;

	virtual void Init() override;
	
	virtual void Update(float _dt) override;

	virtual void Exit() override;

	virtual void SetOwner(std::shared_ptr<GOC> _gameObjPtr);

	virtual void CollisionResponse(std::shared_ptr<GOC> _obj, CollideTypes _type, AABB _aabb, float _dt);

	virtual void UpdateAnimation(std::string _animationStr);

	virtual void LoadResource();
};