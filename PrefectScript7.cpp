/******************************************************************************/
/*
	File Name:		PrefectScript7.cpp
	Project Name:	WhereGotLate
	Author(s):		Lee Shi Lin [100%]

	Brief: Set for Prefect NPC behaviour (Stage 4)

	Copyright Information:
	All content © 2020 DigiPen (SINGAPORE) Corporation, all rights reserved.
*/
/******************************************************************************/
#include "stdafx.h"
#include "PrefectScript7.h"

#include "Logic.h"
#include "CoreEngine.h"
#include "ForceManager.h"


PrefectScript7::PrefectScript7()
	: NPCScript()
{
}

PrefectScript7::~PrefectScript7()
{
}


void PrefectScript7::Start()
{
}

void PrefectScript7::Init()
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

	patrol->AddPatrolPoint(VEC2(2145, 1065));
	patrol->AddPatrolPoint(VEC2(2145, 1835));
	patrol->AddPatrolPoint(VEC2(1315, 1835));
	patrol->AddPatrolPoint(VEC2(1315, 1065));

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
	LOGIC->AddStateMachine("PREFECT_7", tmpSM);
}
