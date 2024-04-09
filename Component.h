/*************************************************************************************/
/*
File Name:		Component.h
Project Name:	WhereGotLate
Author(s):		Kok Zhe Yan			 [100%]


Copyright Information:
All content © 2019 DigiPen (SINGAPORE) Corporation, all rights reserved.
*/
/*************************************************************************************/
#pragma once

#include "Message.h"
#include "ISerialization.h"
#include "ComponentTypeIds.h"


#include "RapidjsonSerialization.h"

// Forward declaration of GameObjectComposition class
class GameObjectComposition;
using GOC = GameObjectComposition;
using CompId = unsigned int;

class Component
{
public:
	friend class GameObjectComposition;

	virtual void Initialize() {};

	virtual std::shared_ptr<Component> Clone() = 0;

	virtual void Inspect() {};

	virtual void SendMessage(Message&){};

	virtual void Serialize(ISerialization&){};

	virtual void Serialize(JsonReader&) {};

	virtual void DeSerialize(JsonWriter&) {};

	virtual void Destroy() = 0;

	void SetBase(std::shared_ptr<GOC> _obj) { mBase = _obj; }

	std::shared_ptr<GOC> GetBase() { return mBase.lock(); }

	void SetID(CompId _id) { mID = _id; }

	CompId GetID() { return mID; }

	void SetName(std::string _name) { mCName = _name; }

	const char* GetName() { return mCName.c_str(); }

	ComponentTypeId TypeId;

protected:
	// Destroy the component
	virtual ~Component() {};
private:
	// Pointer to the base owning composition
	std::weak_ptr<GOC> mBase;

	CompId mID;

	std::string mCName;
};