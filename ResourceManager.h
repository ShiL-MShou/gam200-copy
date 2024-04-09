/*************************************************************************************/
/*
File Name:		ResourceManager.h
Project Name:	WhereGotLate
Author(s):		Kok Zhe Yan			 [100%]


Copyright Information:
All content © 2019 DigiPen (SINGAPORE) Corporation, all rights reserved.
*/
/*************************************************************************************/
#pragma once

#include "Singleton.h"

#include "Composition.h"
//#include "Engine.h"

#define MAX_ENTITY_SIZE 500
#define MAX_COMPONENT_SIZE 3500

class ResourceManager : public Singleton<ResourceManager>
{
	ResourceManager();
	virtual ~ResourceManager();

	ResourceManager(const ResourceManager&) = delete;
	ResourceManager& operator=(const ResourceManager&) = delete;

	friend class Singleton<ResourceManager>;
	friend class GameObjectComposition;
	friend class Component;

	std::vector<std::shared_ptr<GOC>> mTileList;
	std::vector<std::shared_ptr<GOC>> mObjectList;
	std::vector<std::shared_ptr<Component>> mComponentList;
	std::vector<GOCId> mFreedObjectIDs;
	std::vector<CompId> mFreedComponentIDs;

	GOCId mLastObjectID;
	GOCId mLastTileID;
	CompId mLastComponentID;

	std::set<GOCId> mObjectsToBeDeleted;

	void RemoveComponent(CompId _id);
	   
public:
	void Initialize();
	void Update(float _dt);
	void Clear();
	//void SendMessage(Message& _message);
	//std::string GetName() override { return "ResourceManager"; }

	std::shared_ptr<GOC> GetObjectWithID(GOCId _id);
	GOCId AddObject(std::shared_ptr<GOC> _obj);
	GOCId AddTile(std::shared_ptr<GOC> _tile);

	template <typename type = Component>
	std::shared_ptr<type> GetComponentWithID(CompId _id);
	//std::shared_ptr<type> GetComponent(CompId _id);

	void RemoveObject(GOCId _id);

	//std::shared_ptr<Component> GetComponentWithID(CompId _id);
	CompId AddComponent(std::shared_ptr<Component> _component);

	std::vector<std::shared_ptr<GOC>> GetObjMap() { return mObjectList; }

	size_t GetNumObj() { return mLastObjectID - mFreedObjectIDs.size(); }
};

//extern std::shared_ptr<ResourceManager> MYRESOURCE;
#define MyResource (ResourceManager::GetInstance())

template<typename type>
inline std::shared_ptr<type> ResourceManager::GetComponentWithID(CompId _id)
{
	if (_id >= mLastComponentID)
		return nullptr;
	else
		return std::static_pointer_cast<type>(mComponentList[_id]);
}
