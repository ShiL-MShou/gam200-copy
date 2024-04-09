/*************************************************************************************/
/*
File Name:		Composition.h
Project Name:	WhereGotLate
Author(s):		Kok Zhe Yan			 [100%]


Copyright Information:
All content © 2019 DigiPen (SINGAPORE) Corporation, all rights reserved.
*/
/*************************************************************************************/
#pragma once

#include "Message.h"
#include "ISerialization.h"
#include "Component.h"
#include <memory>

// Game Object Id type
using GOCId = unsigned int;
class Factory;
class ResourceManager;

class GameObjectComposition
{

	struct ComponentSort
	{
	public:
		bool operator() (const std::string& lhs, const std::string& rhs) const
		{
			if (strcmp(lhs.c_str(), rhs.c_str()) > 0)
				return true;
			else
				return false;
		}
	};

public:
	// Shared pointer could not access the constructor and destructor if these were private
	GameObjectComposition();
	~GameObjectComposition();

	friend class Factory;
	friend class ResourceManager;

	void SendMessage(Message& _message);

	std::shared_ptr<Component> GetComponent(const std::string& _name);

	template<typename type>
	std::shared_ptr<type> GetComponentType(const std::string& _name);

	void Initialize(std::shared_ptr<GOC> _object);

	std::shared_ptr<GOC> Clone();

	void Destroy();

	void AddComponent(const std::string& _name, CompId _id);

	void RemoveComponent(const std::string& _name);

	GOCId GetId() { return mObjectId; }

	void SetName(std::string _name) { mObjectName = _name; }

	const char* GetName() { return mObjectName.c_str(); }

	void SetType(std::string _type) { mObjectType = _type; }

	const char* GetType() { return mObjectType.c_str(); }

	std::map<std::string, CompId, ComponentSort> GetComponentMap() { return mComponents; }

	void SetInventory(bool _b) { mIsInventory = _b; }
	bool isInventory() { return mIsInventory; }


private:

	std::map<std::string, CompId, ComponentSort> mComponents;

	GOCId mObjectId;

	std::string mObjectName;

	std::string mObjectType;

	bool mIsInventory;
};

template<typename type>
inline std::shared_ptr<type> GameObjectComposition::GetComponentType(const std::string& _name)
{
	return std::static_pointer_cast<type>(GetComponent(_name));
}

#define has(type) GetComponentType<type>( #type )
