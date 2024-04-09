/************************************************************************************ /
/*
File Name:		Script.h
Project Name:	WhereGotLate
Author(s):		Tay Qin Wen Alvin [100%]


Brief:			Base for all the scripts


Copyright Information:
All content © 2019 DigiPen (SINGAPORE) Corporation, all rights reserved.
*/
/*************************************************************************************/

#pragma once

#include "Composition.h"
#include "BoxCollider.h"


class Script
{

protected:
	std::weak_ptr<GOC>			gameObjPtr;

public:
	virtual void Start();
	virtual void Init();
	virtual void Update(float _dt);
	virtual void Exit();

	virtual void SetOwner(std::shared_ptr<GOC> _gameObjPtr);
	std::weak_ptr<GOC> GetOwner();

	virtual void UseItem(const VEC2& _pos, int _frame);

	virtual void CollisionResponse(CollideTypes _type, VEC2 _aabbMin, VEC2 _aabbMax);


	virtual void LoadResource();
};

