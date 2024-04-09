/******************************************************************************/
/*
	File Name:		PrefectScript3.cpp
	Project Name:	WhereGotLate
	Author(s):		Lee Shi Lin [100%]

	Brief: Set for Prefect NPC behaviour (Stage 1)

	Copyright Information:
	All content © 2020 DigiPen (SINGAPORE) Corporation, all rights reserved.
*/
/******************************************************************************/
#include "stdafx.h"
#include "PrefectScript3.h"

#include "Logic.h"
#include "CoreEngine.h"
#include "ForceManager.h"


PrefectScript3::PrefectScript3()
	: NPCScript()
{
}

PrefectScript3::~PrefectScript3()
{
}


void PrefectScript3::Start()
{
	
}

void PrefectScript3::Init()
{
	dir = RIGHT;
	clock->_time = 1.5f;

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
	patrol->AddPatrolPoint(VEC2(1528, 1376));
	patrol->AddPatrolPoint(VEC2(672, 1376));
	
	/*/
	patrol->AddPatrolPoint(VEC2(2790, 415));
	patrol->AddPatrolPoint(VEC2(2790, 1330));
	patrol->AddPatrolPoint(VEC2(3860, 1330));
	patrol->AddPatrolPoint(VEC2(3860, 415));
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
	LOGIC->AddStateMachine("PREFECT_3", tmpSM);
}
