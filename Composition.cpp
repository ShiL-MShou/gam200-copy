/*************************************************************************************/
/*
File Name:		Composition.cpp
Project Name:	WhereGotLate
Author(s):		Kok Zhe Yan			 [100%]


Copyright Information:
All content © 2019 DigiPen (SINGAPORE) Corporation, all rights reserved.
*/
/*************************************************************************************/
#include "stdafx.h"
#include "Composition.h"
#include "Factory.h"
#include "ResourceManager.h"

void GameObjectComposition::SendMessage(Message& _message)
{
	for (auto& [name, id] : mComponents)
	{
		MyResource.GetComponentWithID(id)->SendMessage(_message);
	}
}

std::shared_ptr<Component> GameObjectComposition::GetComponent(const std::string& _name)
{
	if (mComponents.find(_name) != mComponents.end())
		return MyResource.GetComponentWithID(mComponents[_name]);
	else
		return nullptr;
}

void GameObjectComposition::Initialize(std::shared_ptr<GOC> _object)
{
	for (auto [name, id] : mComponents)
	{
		std::shared_ptr<Component> component = MyResource.GetComponentWithID(id);
		component->mBase = _object;
		component->Initialize();
	}
}

std::shared_ptr<GOC> GameObjectComposition::Clone()
{
	std::shared_ptr<GOC> newGOC = std::make_shared<GOC>();

	for (auto& [name, id] : mComponents)
	{
		newGOC->mComponents[name] = FACTORY->CreateComponent(id)->GetID();
	}

	newGOC->mObjectType = mObjectType;

	return newGOC;
}

void GameObjectComposition::Destroy()
{
	MyResource.RemoveObject(mObjectId);
}

void GameObjectComposition::AddComponent(const std::string& _name, CompId _id)
{
	mComponents[_name] = _id;
}

void GameObjectComposition::RemoveComponent(const std::string& _name)
{
	MyResource.RemoveComponent(mComponents[_name]);
	mComponents.erase(_name);
}

GameObjectComposition::GameObjectComposition() : mObjectId{ 0 }, mIsInventory{ false }
{
}

GameObjectComposition::~GameObjectComposition()
{
	mComponents.clear();
}
