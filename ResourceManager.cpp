/*************************************************************************************/
/*
File Name:		ResourceManager.cpp
Project Name:	WhereGotLate
Author(s):		Kok Zhe Yan			 [100%]


Copyright Information:
All content © 2019 DigiPen (SINGAPORE) Corporation, all rights reserved.
*/
/*************************************************************************************/
#include "stdafx.h"

#include "ResourceManager.h"

ResourceManager::ResourceManager() : mLastObjectID{ 0 }, mLastComponentID{ 0 }, mLastTileID{ 0 }
{
}

ResourceManager::~ResourceManager()
{
}

void ResourceManager::Initialize()
{
	//mObjectList.resize(MAX_ENTITY_SIZE);
	//mComponentList.resize(MAX_COMPONENT_SIZE);
}

void ResourceManager::Update(float _dt)
{
	UNREFERENCED_PARAMETER(_dt);
	for (auto& objid : mObjectsToBeDeleted)
	{
		//auto componentmap = mObjectList[id]->mComponents;
		if (objid >= mObjectList.size())
			continue;
		for (auto [type, id] : mObjectList[objid]->mComponents)
		{
			if (mComponentList[id])
				mComponentList[id]->Destroy();
		}

		mObjectList[objid].reset();
		mFreedObjectIDs.push_back(objid);
	}
	mObjectsToBeDeleted.clear();
}

void ResourceManager::Clear()
{
	mObjectsToBeDeleted.clear();
	mTileList.clear();
	mObjectList.clear();
	mComponentList.clear();
	mFreedComponentIDs.clear();
	mFreedObjectIDs.clear();
	mLastComponentID = 0;
	mLastObjectID = 0;
	mLastTileID = 0;
}

std::shared_ptr<GOC> ResourceManager::GetObjectWithID(GOCId _id)
{
	if (_id >= mLastObjectID)
		return nullptr;
	else
		return mObjectList[_id];
}

GOCId ResourceManager::AddObject(std::shared_ptr<GOC> _obj)
{
	if (mFreedObjectIDs.empty())
	{
		mObjectList.push_back(_obj);

		return mLastObjectID++;
	}
	else
	{
		GOCId id = mFreedObjectIDs.back();
		mFreedObjectIDs.pop_back();

		mObjectList[id] = _obj;

		return id;
	}
}

GOCId ResourceManager::AddTile(std::shared_ptr<GOC> _tile)
{
	mTileList.push_back(_tile);

	return mLastTileID++;
}

void ResourceManager::RemoveObject(GOCId _id)
{
	mObjectsToBeDeleted.insert(_id);
}

//std::shared_ptr<Component> ResourceManager::GetComponentWithID(CompId _id)
//{
//	if (_id >= mLastComponentID)
//		return nullptr;
//	else
//		return mComponentList[_id];
//}

CompId ResourceManager::AddComponent(std::shared_ptr<Component> _component)
{
	if (mFreedComponentIDs.empty())
	{
		mComponentList.push_back(_component);
		_component->SetID(mLastComponentID);

		return mLastComponentID++;
	}
	else
	{
		CompId id = mFreedComponentIDs.back();
		mFreedComponentIDs.pop_back();

		mComponentList[id] = _component;
		_component->SetID(id);

		return id;
	}
}

void ResourceManager::RemoveComponent(CompId _id)
{
	mComponentList[_id]->Destroy();
	mComponentList[_id].reset();

	mFreedComponentIDs.push_back(_id);
}
