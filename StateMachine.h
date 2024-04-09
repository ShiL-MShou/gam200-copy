/******************************************************************************/
/*
	File Name: StateMachine.h
	Project Name: WhereGotLate
	Author(s): Lee Shi Lin [100%]

	Brief: StateMachine class holds multiple states for AI

	Copyright Information:
	All content © 2019 DigiPen (SINGAPORE) Corporation, all rights reserved.
*/
/******************************************************************************/
#pragma once
#include "stdafx.h"
#include "State.h"
#include <iostream>
#include <vector>
#include <map>
#include <iostream>
#include <Component.h>

class StateMachine
{
	/*  gamobj component*/ //owner /* ptr to  */
	// goc* m_owner;
	int id;

	std::weak_ptr<GOC> _Owner;
	int _startState;
	int _currState;
	int _prevState;

	bool _activeSM = true;
	float _downtime = 6.0f;
	float _currtime = 0.0f;
	bool changeHappen = false;
	std::vector< std::shared_ptr<iState> > _lState;
	//std::map<std::string, std::shared_ptr<iState> > _lState;

	

public:
	StateMachine();
	~StateMachine();




	bool updatedSprite = false;

	// Create State machine

	// Run current state Update
	void Update(float _dt);

	// Set Name
	void SetID(int _id);

	// Set Owner
	void SetOwner(std::shared_ptr<GOC> own);

	// Get Owner
	std::shared_ptr<GOC> GetOwner();

	// Set Start State
	void SetStartState(int index);

	// Reset State Machine
	void ResetSM();

	// Add State
	void AddState(std::shared_ptr<iState> st);

	// Delete State by index, starting from 0
	void DeleteState(int index);

	// Get State by index
	std::shared_ptr<iState> GetState(int index);

	// Get Current State ptr
	std::shared_ptr<iState> GetCurrState();

	// Get Previous State ptr
	std::shared_ptr<iState> GetPrevState();

	// Get Current State index
	int GetCurrStateindex();

	// Get Previous State index
	int GetPrevStateindex();

	// Get Transition
	std::shared_ptr<iTransition> GetTransition(int si, std::string ti);

	// Pass check
	void SendDistractMsgSM(bool s, VEC2 l, int starget, std::string ttarget);

	// Pause Action
	void CollideWithItem();

	// Set State Machine status. (Active/Paused)
	void ActiveSM(bool);

	// Set Downtime
	void SetDownTime(float t);
};
