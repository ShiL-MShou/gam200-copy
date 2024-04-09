/******************************************************************************/
/*
	File Name:		TeacherScript.cpp
	Project Name:	WhereGotLate
	Author(s):		Lee Shi Lin [100%]

	Brief: Set for Teacher NPC behaviour

	Copyright Information:
	All content © 2020 DigiPen (SINGAPORE) Corporation, all rights reserved.
*/
/******************************************************************************/
#pragma once
#include "NPCScript.h"


class TeacherScript : public NPCScript
{
	// Create States
	std::shared_ptr<IDLE_STATE> idle = std::make_shared<IDLE_STATE>();
	std::shared_ptr<PATROL_STATE> patrol = std::make_shared<PATROL_STATE>();
	std::shared_ptr<DISTRACTED_STATE> distract = std::make_shared<DISTRACTED_STATE>();
	std::shared_ptr<CHASE_STATE> chase    = std::make_shared<CHASE_STATE>();

	// create condition
	std::shared_ptr<TimerTransition> clock		= std::make_shared<TimerTransition>(Patrol);
	std::shared_ptr<Rest> toRest				= std::make_shared<Rest>(Idle);
	std::shared_ptr<ReachItem> RI				= std::make_shared<ReachItem>(Patrol);
	std::shared_ptr<Distracted> Found1			= std::make_shared<Distracted>(Distract);
	std::shared_ptr<Distracted> Found2			= std::make_shared<Distracted>(Distract);
	std::shared_ptr<GiveUpChasing> stop			= std::make_shared<GiveUpChasing>(Patrol);
	std::shared_ptr<DetectPlayer> seePlayI      = std::make_shared<DetectPlayer>(Chase);
	std::shared_ptr<DetectPlayer> seePlayP      = std::make_shared<DetectPlayer>(Chase);
	std::shared_ptr<DetectPlayer> seePlayD      = std::make_shared<DetectPlayer>(Chase);


public:
	//TeacherScript() : sm{}

	virtual void Start() override;

	void Init() override;

	//virtual void Update(float _dt) override;

	//virtual void Exit() override;

	//virtual void SetOwner(std::shared_ptr<GOC> _gameObjPtr) override;

	//virtual void CollisionResponse(std::weak_ptr<GOC> _obj, CollideTypes _type, AABB _aabb, float _dt) override;

	virtual void UpdateAnimation(std::string _animationStr) override;
};

