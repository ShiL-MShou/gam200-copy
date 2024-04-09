/******************************************************************************/
/*
	File Name:		PrefectScript6.cpp
	Project Name:	WhereGotLate
	Author(s):		Lee Shi Lin [100%]

	Brief: Set for Prefect NPC behaviour (Stage 4)

	Copyright Information:
	All content © 2020 DigiPen (SINGAPORE) Corporation, all rights reserved.
*/
/******************************************************************************/
#include "stdafx.h"
#include "PrefectScript6.h"

#include "Logic.h"
#include "CoreEngine.h"
#include "ForceManager.h"


PrefectScript6::PrefectScript6()
	: NPCScript()
{
}

PrefectScript6::~PrefectScript6()
{
}


void PrefectScript6::Start()
{
}

void PrefectScript6::Init()
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

	patrol->AddPatrolPoint(VEC2(867, 2800));
	patrol->AddPatrolPoint(VEC2(867, 1835));
	patrol->AddPatrolPoint(VEC2(1760, 1835));
	patrol->AddPatrolPoint(VEC2(1760, 2800));

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
	LOGIC->AddStateMachine("PREFECT_6", tmpSM);
}
