/******************************************************************************/
/*
	File Name: State.h
	Project Name: WhereGotLate
	Author(s): Lee Shi Lin [100%]

	Brief: Interface class to all states

	Copyright Information:
	All content © 2019 DigiPen (SINGAPORE) Corporation, all rights reserved.
*/
/******************************************************************************/
#pragma once
#include "stdafx.h"
#include <iostream>
#include "Transition.h"
#include <string>
#include <vector>
#include <map>
#include "Body.h"


class StateMachine;

class iState
{
	//int ownerID;
protected:
	StateMachine* _ownerSM;
	//std::vector< std::shared_ptr<iTransition>> _lTransition;
	std::map<std::string, std::shared_ptr<iTransition>> _lTransition;


public:

	std::string _name;

	virtual void Update(float _dt);
	virtual void OnEnter();
	virtual void OnExit();

	virtual void UpdateAnimation();

	virtual void ResetState();

	// Add iTransition
	virtual void AddTransition(std::string key, std::shared_ptr<iTransition> condi);

	// Delete iTransition
	void DeleteTransiion(std::string key);
	
	// Check all condition, return next state to change 
	int TryTransit(float _dt);

	void SetOwner(StateMachine* _owner);

	std::shared_ptr<GOC> GetOwner();

	StateMachine* GetSMOwner();

	// Get iTransition
	std::shared_ptr<iTransition> GetTransition(std::string index);
	iTransition* GetRawTransitionIndex(std::string index);

	// Send Distract message
	void SendDistractMsgS(bool s, VEC2 locate, std::string index);

	void MoveToPos(std::shared_ptr<Body> _bodyPtr, VEC2& _currPos, VEC2& _destPos);

	//void MoveToPos(std::shared_ptr<Body> _bodyPtr, VEC2& _currPos, VEC2& _destPos, VEC2& _nextDes);

	bool IsUnableToChase();

	void DisableAllForces(std::shared_ptr<Body> _bodyPtr);

};