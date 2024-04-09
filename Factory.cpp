/*************************************************************************************/
/*
File Name:		Factory.cpp
Project Name:	WhereGotLate
Author(s):		Kok Zhe Yan			 [100%]


Copyright Information:
All content © 2019 DigiPen (SINGAPORE) Corporation, all rights reserved.
*/
/*************************************************************************************/
#include "stdafx.h"
#include "Factory.h"
#include "Composition.h"
#include "ComponentCreator.h"
#include "TextSerialization.h"
#include <filesystem>

#include "RapidjsonSerialization.h"
#include "ResourceManager.h"
#include "CoreEngine.h"


std::shared_ptr<Factory> FACTORY;
namespace fs = std::filesystem;

Factory::Factory()
{
	//mlastObjectId = 0;
}

Factory::~Factory()
{
	//mObjectIds.clear();
	mComponentCreators.clear();
	mArcheTypes.clear();
}

void Factory::Initialize()
{
	MyResource.Initialize();
}

void Factory::Update(float _dt)
{
	UNREFERENCED_PARAMETER(_dt);
	MyResource.Update(_dt);
}

void Factory::SendMessage(std::unique_ptr<Message> _message)
{
}

std::shared_ptr<GOC> Factory::CreateObject(const std::string& _archetype)
{
	std::map<std::string, std::shared_ptr<GOC>>::iterator iter = mArcheTypes.find(_archetype);
	assert(iter != mArcheTypes.end() && "No Archetype of this object found.");
	
	std::shared_ptr<GOC> object = iter->second->Clone();
	
	return object;
}

std::shared_ptr<Component> Factory::CreateComponent(CompId _id)
{
	std::shared_ptr<Component> blueprint = MyResource.GetComponentWithID(_id);
	std::shared_ptr<Component> newcomponent = blueprint->Clone();
	newcomponent->SetName(blueprint->GetName());
	newcomponent->TypeId = blueprint->TypeId;
	MyResource.AddComponent(newcomponent);

	return newcomponent;
}


void Factory::CreateArcheType(const std::string& _name)
{
	if (mArcheTypes.find(_name) == mArcheTypes.end())
		mArcheTypes[_name] = ConstructObject(_name);
}

void Factory::Destroy(std::shared_ptr<GOC> _object)
{
	//mObjectsToBeDeleted.insert(_object);
	mArcheTypes.clear();
}

void Factory::Clear()
{
	//mObjectIds.clear();
	//mlastObjectId = 0;
	mArcheTypes.clear();
	MyResource.Clear();
}

std::shared_ptr<GOC> Factory::ConstructEmpty()
{
	std::shared_ptr<GOC> object = std::make_shared<GOC>();
	return object;
}

std::shared_ptr<GOC> Factory::ConstructObject(const std::string& _filename)
{
	JsonReader jr(CORE->mPath + "Objects/" + _filename + ".json");
	bool isFileOpened = jr.IsFileOpened();
	assert(isFileOpened && "This file cannot be opened.");

	jr.ParseStreamToDoc();
	jr.doc.Accept(jr.prettyReaderBuffer);

	if (!isFileOpened)
	{
		std::cout << "Error Loading level!" << std::endl;
		assert(0);
	}
	else
	{
		//needed to reset the ifstream good() flag, otherwise for some reason the below jr.IsGood() will be false
		jr.stream.clear();
	}

	std::string objectname;

	if (jr.IsGood())
	{
		std::shared_ptr<GOC> object = std::make_shared<GOC>();
		//while (jr.IsGood())
		{
			//std::cout << _filename << std::endl;

			//append whatever count is to the end of the Key
			if (jr.doc.HasMember("Name"))
				jr.ReadStringMember("Name", "Name", objectname);
			//std::cout << objectname << std::endl;
			

			for (auto [name, creatorPtr] : mComponentCreators)
			{
				if (jr.doc.HasMember(name.c_str()) )
				{
					// get the component creator from the map
					//std::shared_ptr<ComponentCreator> creator = creatorPtr;
					// create the component with the creator
					std::shared_ptr<Component> component = ConstructComponent(name);
					//std::cout << name << std::endl;
					// serialize the component
					StreamRead(jr, *component);
					// add the new component to the object composition
					object->AddComponent(name, component->GetID());
					object->SetName(objectname);
					object->SetType(objectname);
				}
			}

			//std::cout << std::endl;
		}
		return object;
	}
	return NULL;
}

std::shared_ptr<Component> Factory::ConstructComponent(const std::string& _componentname)
{
	std::shared_ptr<Component> newcomponent = mComponentCreators[_componentname]->Create();
	MyResource.AddComponent(newcomponent);
	newcomponent->SetName(_componentname);

	return newcomponent;
}

void Factory::IDObject(std::shared_ptr<GOC> _object)
{
	//_object->mObjectId = ++mlastObjectId;

	//mObjectIds[mlastObjectId] = _object;

	_object->mObjectId = MyResource.AddObject(_object);
}

void Factory::IDTile(std::shared_ptr<GOC> _tile)
{
	_tile->mObjectId = MyResource.AddTile(_tile);
}

void Factory::AddComponentCreator(const std::string& _name, std::shared_ptr<ComponentCreator> _creator)
{
	mComponentCreators[_name] = _creator;
}

std::shared_ptr<GOC> Factory::GetObjectWithID(GOCId _id)
{
	// return object if found, else return NULL
	//return mObjectIds.find(_id) != mObjectIds.end() ?	mObjectIds[_id] : NULL;
	return MyResource.GetObjectWithID(_id);
}

void Factory::AddArcheType(const std::string& _name)
{
	assert(mArcheTypes.find(_name) == mArcheTypes.end());

	std::shared_ptr<GOC> newobj = FACTORY->ConstructEmpty();
	newobj->SetName(_name);

	mArcheTypes[_name] = newobj;
}

void Factory::SaveArchetype(const std::string& _filename)
{
	std::string file = CORE->mPath + "Objects/" + _filename + ".json";

	JsonWriter jw;

	jw.prettyWriterBuffer.StartObject();

	jw.CreateKey("Name");
	jw.prettyWriterBuffer.StartObject();
	jw.CreateStringMember("Name", _filename);
	jw.prettyWriterBuffer.EndObject();

	if (mArcheTypes[_filename]->mComponents.size() != 0)
	{
		auto componentmap = mArcheTypes[_filename]->mComponents;

		for (auto [type, id] : componentmap)
		{
			StreamWrite(jw, *MyResource.GetComponentWithID(id));
		}
	}
	jw.prettyWriterBuffer.EndObject();

	jw.WriteToJsonFile(file);
}


