/************************************************************************************ /
/*
File Name:		PrefectScript1.cpp
Project Name:	WhereGotLate
Author(s):		Tay Qin Wen Alvin [100%]

Brief:			Script for prefect1


Copyright Information:
All content © 2020 DigiPen (SINGAPORE) Corporation, all rights reserved.
*/
/*************************************************************************************/

#include "stdafx.h"
#include "PrefectScript1.h"

#include "Logic.h"
#include "CoreEngine.h"
#include "ForceManager.h"


PrefectScript1::PrefectScript1()
	: NPCScript()
{
}

PrefectScript1::~PrefectScript1()
{
}


void PrefectScript1::Start()
{
}

void PrefectScript1::Init()
{
	dir = LEFT;

	clock->_time = 1.5f;
	//
	//std::shared_ptr<StateMachine> tmpSM = std::make_shared<StateMachine>();

	//// Add Transition to States
	//idle->AddTransition("walk", clock);
	//idle->AddTransition("distracted", Found1);

	//patrol->AddTransition("idling", toRest);
	//patrol->AddTransition("distracted", Found2);
	//patrol->AddTransition("chase", seePlayI);

	//chase->AddTransition("giveup", stop);

	//distract->AddTransition("stopDistract", RI);

	std::shared_ptr<StateMachine> tmpSM = std::make_shared<StateMachine>();

	dir = 0;

	// Add Transition to States
	//idle->AddTransition("walk", clock);
	//idle->AddTransition("distracted", Found1);
	idle->AddTransition("chase", seePlayI);

	//patrol->AddTransition("idling", toRest);
	//patrol->AddTransition("distracted", Found2);
	//patrol->AddTransition("chase", seePlayP);

	//distract->AddTransition("stopDistract", RI);
	//distract->AddTransition("chase", seePlayD);

	chase->AddTransition("giveup", stop2);

	// Add Patrol points
	//patrol->AddPatrolPoint(VEC2(1134, 2200));
	//patrol->AddPatrolPoint(VEC2(1134, 1590));
	//patrol->AddPatrolPoint(VEC2(1645, 1590));
	//patrol->AddPatrolPoint(VEC2(1645, 2200));

	//patrol->AddPatrolPoint(VEC2(1580, 470));
	//patrol->AddPatrolPoint(VEC2(1134, 1590));

	//patrol->SetStartPoint();

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
	LOGIC->AddStateMachine("PREFECT_1", tmpSM);
}
