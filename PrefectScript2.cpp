/******************************************************************************/
/*
	File Name:		PrefectScript2.cpp
	Project Name:	WhereGotLate
	Author(s):		Lee Shi Lin [100%]

	Brief: Set for Prefect NPC behaviour (Stage 1)

	Copyright Information:
	All content © 2020 DigiPen (SINGAPORE) Corporation, all rights reserved.
*/
/******************************************************************************/
#include "stdafx.h"
#include "PrefectScript2.h"

#include "Logic.h"
#include "CoreEngine.h"
#include "ForceManager.h"


PrefectScript2::PrefectScript2()
	: NPCScript(), bodyPtr{ nullptr }, spritePtr{ nullptr },
		idle{ std::make_shared<IDLE_STATE>() },
		patrol{ std::make_shared<PATROL_STATE>() },
		distract{ std::make_shared<DISTRACTED_STATE>() },
		chase{ std::make_shared<CHASE_STATE>() },
		clock{ std::make_shared<TimerTransition>(Patrol) },
		toRest{ std::make_shared<Rest>(Idle) },
		RI{ std::make_shared<ReachItem>(Patrol) },
		Found1{ std::make_shared<Distracted>(Distract) },
		Found2{ std::make_shared<Distracted>(Distract) },
		stop{ std::make_shared<GiveUpChasing>(Patrol) },
		seePlayI{ std::make_shared<DetectPlayer>(Chase) },
		seePlayP{ std::make_shared<DetectPlayer>(Chase) },
		seePlayD{ std::make_shared<DetectPlayer>(Chase) }
{
}

PrefectScript2::~PrefectScript2()
{
}


void PrefectScript2::Start()
{
	
}

void PrefectScript2::Init()
{
	dir = RIGHT;
	clock->_time = 1.5f;

	// -------------------------
	std::shared_ptr<StateMachine> tmpSM = std::make_shared<StateMachine>();

	// Add Transition to States
	idle->AddTransition("walk", clock);
	idle->AddTransition("distracted", Found1);
	idle->AddTransition("chase", seePlayI);

	patrol->AddTransition("idling", toRest);
	patrol->AddTransition("distracted", Found2);
	patrol->AddTransition("chase", seePlayP);

	distract->AddTransition("stopDistract", RI);
	distract->AddTransition("chase", seePlayD);

	chase->AddTransition("giveup", stop);

	// Add Patrol points
	patrol->AddPatrolPoint(VEC2(1380, 1270));
	patrol->AddPatrolPoint(VEC2(544, 1270));
	
	/*/
	patrol->AddPatrolPoint(VEC2(2780, 2200));
	patrol->AddPatrolPoint(VEC2(2780, 1370));
	patrol->AddPatrolPoint(VEC2(1670, 1370));
	patrol->AddPatrolPoint(VEC2(1670, 2200));
	/*/
	patrol->SetStartPoint();

	// teacher movement force
	MyForceManager.CreateMovementForces(20000.0f, 0.018f, "teacher");
	//bodyPtr->SetMaxSpeed();

	// Population to state machine
	tmpSM->AddState(idle);     // 0
	tmpSM->AddState(patrol);   // 1
	tmpSM->AddState(distract); // 2
	tmpSM->AddState(chase);    // 3

	tmpSM->SetStartState(0);

	// Add State Machine to Logic System
	LOGIC->AddStateMachine("PREFECT_2", tmpSM);
}

