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
#include "stdafx.h"
#include "StateMachine.h"

#include "Logic.h"

StateMachine::StateMachine()
	: id{ -1 }, _Owner{}, _startState{0}, _currState{0}, _prevState{-1}, _activeSM{true}, _downtime{6.0f}, _currtime{0.0f},
	changeHappen{false}, _lState{}
{
}

StateMachine::~StateMachine()
{
}

void StateMachine::Update(float _dt)
{
	if (_activeSM)
	{
		// Check if first time entering state
		if (changeHappen)
		{
			_lState[_currState]->OnEnter();
			changeHappen = false;
			return;
		}

		_lState[_currState]->Update(_dt);

		int next = _lState[_currState]->TryTransit(_dt);

		// Check if there is any change in state
		if (next != -1)
		{
			_lState[_currState]->OnExit();
			_prevState = _currState;
			_currState = next;
			changeHappen = true;
		}

	}
}

void StateMachine::SetID(int _id)
{
	id = _id;
}

void StateMachine::SetOwner(std::shared_ptr<GOC> own)
{
	_Owner = own;
}

std::shared_ptr<GOC> StateMachine::GetOwner()
{
	return _Owner.lock();
}

void StateMachine::SetStartState(int index)
{
	_startState = index;
	_currState = _startState;
}

void StateMachine::ResetSM()
{
	_currState = _startState;
	_prevState = _currState;

	//LOGIC->CanDistract();

	/* Loop through all state, reset their variables*/
	for (size_t i = 0; i < _lState.size(); i++)
		_lState[i]->ResetState();

	//LOGIC->CanDistract();
}

void StateMachine::AddState(std::shared_ptr<iState> st)
{
	// st->SetOwner(id);
	st->SetOwner(this);
	_lState.push_back(st);
}

void StateMachine::DeleteState(int index)
{
	_lState.erase(_lState.begin() + index);
}

std::shared_ptr<iState> StateMachine::GetState(int index)
{
	return _lState[index];
}

std::shared_ptr<iState> StateMachine::GetCurrState()
{
	return _lState[_currState];
}

std::shared_ptr<iState> StateMachine::GetPrevState()
{
	return _lState[_prevState];
}

int StateMachine::GetCurrStateindex()
{
	return _currState;
}

int StateMachine::GetPrevStateindex()
{
	return _prevState;
}

std::shared_ptr<iTransition> StateMachine::GetTransition(int si, std::string ti)
{
	return _lState[si]->GetTransition(ti);
}

void StateMachine::SendDistractMsgSM(bool s, VEC2 location, int starget, std::string ttarget)
{
	_lState[starget]->SendDistractMsgS(s, location, ttarget);
}

void StateMachine::CollideWithItem()
{
	_activeSM = false;
}

void StateMachine::ActiveSM(bool b)
{
	_activeSM = b;
}

void StateMachine::SetDownTime(float t)
{
	_downtime = t;
}

