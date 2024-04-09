/*************************************************************************************/
/*
File Name:		System.h
Project Name:	WhereGotLate
Author(s):		Kok Zhe Yan			 [100%]


Copyright Information:
All content © 2019 DigiPen (SINGAPORE) Corporation, all rights reserved.
*/
/*************************************************************************************/
#pragma once

#include "Message.h"

class ResourceManager;

class ISystem
{
	friend class ResourceManager;

public:
	virtual void SendMessage(Message&) {};

	virtual void Update(float _dt) = 0;

	virtual std::string GetName() = 0;

	virtual void Initialize() {};

	virtual void Clear() = 0;

	virtual ~ISystem() {};
};