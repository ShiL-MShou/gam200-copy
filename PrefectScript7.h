/******************************************************************************/
/*
	File Name:		PrefectScript7.h
	Project Name:	WhereGotLate
	Author(s):		Lee Shi Lin [100%]

	Brief: Set for Prefect NPC behaviour (Stage 4)

	Copyright Information:
	All content © 2020 DigiPen (SINGAPORE) Corporation, all rights reserved.
*/
/******************************************************************************/
#pragma once
#include "NPCScript.h"

#include <memory>
#include "StateMachine.h"
#include "Body.h"
#include "SpriteComponent.h"

#include "List_StateLinker.h"
#include "PATROL_STATE.h"
#include "IDLE_STATE.h"
#include "DISTRACTED_STATE.h"
#include "CHASE_STATE.h"
#include "StateEnum.h"


class PrefectScript7 : public NPCScript
{
	bool isRight;
	bool isUp;

	std::shared_ptr<Body>	bodyPtr;
	std::shared_ptr<SpriteComponent> spritePtr;



	// Create States
	std::shared_ptr<IDLE_STATE> idle = std::make_shared<IDLE_STATE>();
	std::shared_ptr<PATROL_STATE> patrol = std::make_shared<PATROL_STATE>();
	std::shared_ptr<DISTRACTED_STATE> distract = std::make_shared<DISTRACTED_STATE>();
	std::shared_ptr<CHASE_STATE> chase = std::make_shared<CHASE_STATE>();

	// create condition
	std::shared_ptr<TimerTransition> clock = std::make_shared<TimerTransition>(Patrol);
	std::shared_ptr<Rest> toRest = std::make_shared<Rest>(Idle);
	std::shared_ptr<ReachItem> RI = std::make_shared<ReachItem>(Patrol);
	std::shared_ptr<Distracted> Found1 = std::make_shared<Distracted>(Distract);
	std::shared_ptr<Distracted> Found2 = std::make_shared<Distracted>(Distract);
	std::shared_ptr<GiveUpChasing> stop = std::make_shared<GiveUpChasing>(Patrol);
	std::shared_ptr<DetectPlayer> seePlayI = std::make_shared<DetectPlayer>(Chase);
	std::shared_ptr<DetectPlayer> seePlayP = std::make_shared<DetectPlayer>(Chase);
	std::shared_ptr<DetectPlayer> seePlayD = std::make_shared<DetectPlayer>(Chase);

	//// idle:0, patrol:1, distract:2, chase:3
	//std::shared_ptr<StateMachine> sm;// = std::make_shared<StateMachine>();

public:
	PrefectScript7();
	~PrefectScript7();

	// delete
	PrefectScript7(const PrefectScript7&) = delete;
	PrefectScript7& operator=(const PrefectScript7&) = delete;

	void Start() override;

	void Init() override;
	
};

