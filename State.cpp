/******************************************************************************/
/*
	File Name: State.cpp
	Project Name: WhereGotLate
	Author(s): Lee Shi Lin [100%]

	Brief: Interface class to all states

	Copyright Information:
	All content © 2019 DigiPen (SINGAPORE) Corporation, all rights reserved.
*/
/******************************************************************************/
#include "stdafx.h"
#include "State.h"
#include "Composition.h"

#include "StateMachine.h"
#include "List_StateLinker.h"

#include "AnimationComponent.h"
#include "Graphics.h"


void iState::Update(float _dt)
{
	UNREFERENCED_PARAMETER(_dt);
}

void iState::OnEnter()
{
}

void iState::OnExit()
{
}

void iState::UpdateAnimation()
{
}

void iState::ResetState()
{
}

void iState::AddTransition(std::string key, std::shared_ptr<iTransition> condi)
{
	condi->SetOwner(this);
	//_lTransition.push_back(condi);
	_lTransition.insert({ key, condi });
}

void iState::DeleteTransiion(std::string key)
{
	//_lTransition.erase(_lTransition.begin() + index);
	_lTransition.erase(key);
}

int iState::TryTransit(float _dt)
{
	int next = -1;
	// Loop through all conditions
	/*/
	for (size_t i = 0; i < _lTransition.size(); i++)
	{
		if (_lTransition[i]->Condition(_dt)) {
			next = _lTransition[i]->GetNextStateNum();
		}
			
	}
	/*/
	std::map<std::string, std::shared_ptr<iTransition>>::iterator it;

	for (it = _lTransition.begin(); it!=_lTransition.end(); ++it)
	{
		if (it->second->Condition(_dt))
		{
			next = it->second->GetNextStateNum();
		}
	}

	return next;
}

void iState::SetOwner(StateMachine* _owner)
{
	_ownerSM = _owner;
}

std::shared_ptr<GOC> iState::GetOwner()
{
	return _ownerSM->GetOwner();
}

StateMachine* iState::GetSMOwner()
{
	return _ownerSM;
}

std::shared_ptr<iTransition> iState::GetTransition(std::string index)
{
	if (_lTransition.find(index) != std::end(_lTransition))
		return _lTransition[index];
	else
		return nullptr;
}

iTransition* iState::GetRawTransitionIndex(std::string index)
{
	if (_lTransition.find(index) != std::end(_lTransition))
		return _lTransition[index].get();
	else
		return nullptr;
}

void iState::SendDistractMsgS(bool s, VEC2 locate, std::string index)
{
	//std::dynamic_pointer_cast<std::shared_ptr<Distracted>>(_lTransition[index]);
	Distracted* here = dynamic_cast<Distracted*>(_lTransition[index].get());
	here->DistractMsg(s, locate);
}

void iState::MoveToPos(std::shared_ptr<Body> _bodyPtr, VEC2& _currPos, VEC2& _destPos)
{
	VEC2 diff = _destPos - _currPos;

	float epsilon = 1.5f;

	bool moved = false;

	// Snap to location
	if (VEC2Magnitude(_currPos, _destPos) <= epsilon)
	{
		//std::cout << "Snap" << std::endl;
		GetOwner()->has(Transform)->SetPosition(_destPos);
	
		GetOwner()->has(Body)->DeactivateForces("playerRight");
		GetOwner()->has(Body)->DeactivateForces("playerLeft");
		GetOwner()->has(Body)->DeactivateForces("playerUp");
		GetOwner()->has(Body)->DeactivateForces("playerDown");
		GetOwner()->has(Body)->DeactivateForces("drag");
	
		GetOwner()->has(Body)->SetVelocity(VEC2(0, 0));
		//GetNextsmallPoint();
		return;
	}

	// Snap X
	if (-epsilon <= diff.x && diff.x <= epsilon)
	{
		//std::cout << "Snap X" << std::endl;

		GetOwner()->has(Body)->DeactivateForces("playerRight");
		GetOwner()->has(Body)->DeactivateForces("playerLeft");
		//moved = false;

		GetOwner()->has(Body)->SetPosX(_destPos.x);
		GetOwner()->has(Body)->SetVelocityX(0.0f);
	}
	else if (_currPos.x < _destPos.x) // move right
	{
		//std::cout << "right" << std::endl;
		GetOwner()->has(Body)->ActivateForces("playerRight");
		GetOwner()->has(Body)->ActivateForces("drag");
		moved = true;
	}
	else if (_currPos.x > _destPos.x) // move left
	{
		//std::cout << "left " << std::endl;
		GetOwner()->has(Body)->ActivateForces("playerLeft");
		GetOwner()->has(Body)->ActivateForces("drag");
		moved = true;
	}

	// Snap Y
	if (-epsilon <= diff.y && diff.y <= epsilon)
	{
		//std::cout << "Snap Y" << std::endl;

		GetOwner()->has(Body)->DeactivateForces("playerUp");
		GetOwner()->has(Body)->DeactivateForces("playerDown");
		//GetOwner()->has(Body)->DeactivateForces("drag");
		

		GetOwner()->has(Body)->SetPosY(_destPos.y);
		GetOwner()->has(Body)->SetVelocityY(0.0f);
	}
	else if (_currPos.y < _destPos.y) // move up
	{
		//std::cout << "Up " << std::endl;
		GetOwner()->has(Body)->ActivateForces("playerUp");
		GetOwner()->has(Body)->ActivateForces("drag");
		moved = true;
	}
	else if (_currPos.y > _destPos.y) // move down
	{
		//std::cout << "Down" << std::endl;
		GetOwner()->has(Body)->ActivateForces("playerDown");
		GetOwner()->has(Body)->ActivateForces("drag");
		moved = true;
	}

	if (moved == false)
		GetOwner()->has(Body)->DeactivateForces("drag");

}


//// MoveToPos with next dest
//void iState::MoveToPos(std::shared_ptr<Body> _bodyPtr, VEC2& _currPos, VEC2& _destPos, VEC2& _nextDes)
//{
//	VEC2 diff = _destPos - _currPos;
//
//	bool moved = false;
//
//
//
//}

bool iState::IsUnableToChase()
{
	std::shared_ptr<SpriteComponent> sprPtr = GetOwner()->has(SpriteComponent);
	std::shared_ptr<AnimationComponent> animationCompPtr = GetOwner()->has(AnimationComponent);

	// safety checks
	if (sprPtr != nullptr && animationCompPtr != nullptr)
	{
		// change idle to teaching
		int animationID = animationCompPtr->animationList.at(animationCompPtr->currAnimation);
		std::string animationName = GRAPHICS->GetAnimationName(animationID);

		if (animationName.find("FALLING") != std::string::npos ||
			animationName.find("STUNNED") != std::string::npos ||
			animationName.find("GETTING_UP") != std::string::npos)
		{
			return true;
		}
	}

	return false;
}

void iState::DisableAllForces(std::shared_ptr<Body> _bodyPtr)
{
	_bodyPtr->DeactivateForces("playerRight");
	_bodyPtr->DeactivateForces("playerLeft");
	_bodyPtr->DeactivateForces("playerUp");
	_bodyPtr->DeactivateForces("playerDown");
	_bodyPtr->DeactivateForces("drag");

	_bodyPtr->SetVelocity(VEC2(0, 0));
}

