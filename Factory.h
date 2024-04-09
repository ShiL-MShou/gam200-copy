/*************************************************************************************/
/*
File Name:		Factory.cpp
Project Name:	WhereGotLate
Author(s):		Kok Zhe Yan			 [100%]


Copyright Information:
All content © 2019 DigiPen (SINGAPORE) Corporation, all rights reserved.
*/
/*************************************************************************************/
#pragma once
#include "System.h"
#include "Composition.h"
//#include "VMath.h"

class ComponentCreator;

class Factory : public ISystem
{
public:

	Factory();
	~Factory();

	void Initialize();
	// Updates the Factory, destroying dead objects
	virtual void Update(float _dt) override;
	// Get the name of the system
	virtual std::string GetName() override { return "Factory"; };
	virtual void SendMessage(std::unique_ptr<Message> _message);

	// Create and init and ID a GOC from a data file
	std::shared_ptr<GOC> CreateObject(const std::string& _objectname);
	// Create a component with the name
	std::shared_ptr<Component> CreateComponent(CompId _id);
	// Create a archetype
	void CreateArcheType(const std::string& _name);
	// Destroy all the game objects upon called when loading new level
	void Clear() override;

	// Used to dynamically create GOC
	// Init with GOC->Initialize() after components have been added 
	std::shared_ptr<GOC> ConstructEmpty();

	// Make a composition and serialize	from the input data file	
	std::shared_ptr<GOC> ConstructObject(const std::string& _filename);
	// Make a new component
	std::shared_ptr<Component> ConstructComponent(const std::string& _componentname);

	// Gives the object an ID and stores it in the map of objects
	void IDObject(std::shared_ptr<GOC> _object);

	// Gives the environment tile an ID and stores it in the map of tiles
	void IDTile(std::shared_ptr<GOC> _tile);

	// Add a component creator enabling data driven composition
	void AddComponentCreator(const std::string& _name, std::shared_ptr<ComponentCreator> _creator);

	// Return the object with the given ID
	std::shared_ptr<GOC> GetObjectWithID(GOCId _id);
	
	// Add a new archetype
	void AddArcheType(const std::string& _name);

	void SaveArchetype(const std::string& _filename);

	// Return the Map of Objects
	//std::map<unsigned, std::shared_ptr<GOC>> GetObjMap() { return mObjectIds; }

	// Return the Map of ArcheTypes
	std::map<std::string, std::shared_ptr<GOC>> GetArcheTypeMap() { return mArcheTypes; }

	// Return the Map of Component Creators
	std::map<std::string, std::shared_ptr<ComponentCreator>> GetCreatorsMap() { return mComponentCreators; }


private:
	// used incrementally to create unique object id
	//unsigned mlastObjectId;
	
	// map storing the Component creator 
	std::map<std::string, std::shared_ptr<ComponentCreator>> mComponentCreators;

	// map of gocid to their objects
	//std::map<unsigned, std::shared_ptr<GOC>> mObjectIds;

	// archetypes of objects
	std::map<std::string, std::shared_ptr<GOC>> mArcheTypes;


	// Add a GOC to the destroy list to be destroyed
	void Destroy(std::shared_ptr<GOC> _object);

	// Set holding Objects to be deleted
	//std::set<std::shared_ptr<GOC>> mObjectsToBeDeleted;
};

extern std::shared_ptr<Factory> FACTORY;