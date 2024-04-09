/******************************************************************************/
/*
	File Name:		TeacherScript.cpp
	Project Name:	WhereGotLate
	Author(s):		Lee Shi Lin [80%]
					Ong Cheng Kai Marcus [20%]

	Brief: Set for Teacher NPC behaviour

	Copyright Information:
	All content © 2020 DigiPen (SINGAPORE) Corporation, all rights reserved.
*/
/******************************************************************************/
#include "stdafx.h"
#include "TeacherScript.h"
#include "ForceManager.h"


void TeacherScript::Start()
{
}

void TeacherScript::Init()
{
	dir = RIGHT;

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

	//if (strcmp(CORE->GetCurrLevel().c_str(), "MainMenu") == 0) //if previous level changes, make sure this is changed too
	//{//have to compare to previous level b/c the function is called in the previous level
	if (strcmp(CORE->GetCurrLevel().c_str(), "Stage2v2") == 0)
	{
		patrol->AddPatrolPoint(VEC2(966, 1070));
		patrol->AddPatrolPoint(VEC2(1101, 1070));
	}
	else if (strcmp(CORE->GetCurrLevel().c_str(), "Stage3v2") == 0)
	{
		patrol->AddPatrolPoint(VEC2(1868, 1055));
		patrol->AddPatrolPoint(VEC2(2011, 1055));
	}
	else if (strcmp(CORE->GetCurrLevel().c_str(), "Stage4") == 0)
	{
		patrol->AddPatrolPoint(VEC2(647, 2175));
		patrol->AddPatrolPoint(VEC2(235, 2175));
	}

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
	LOGIC->AddStateMachine("NPC1", tmpSM);
}

void TeacherScript::UpdateAnimation(std::string _animationStr)
{

}
