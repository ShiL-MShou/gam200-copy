/*************************************************************************************/
/*
File Name:		ComponentCreator.h
Project Name:	WhereGotLate
Author(s):		Kok Zhe Yan			 [100%]


Copyright Information:
All content © 2019 DigiPen (SINGAPORE) Corporation, all rights reserved.
*/
/*************************************************************************************/
#pragma once

#include "ComponentTypeIds.h"
#include <memory>

class Component;

// Game Component Creator Interface
class ComponentCreator
{
public:
	ComponentCreator(ComponentTypeId _typeId)
		: TypeId(_typeId)
	{
	}

	ComponentTypeId TypeId;

	virtual std::shared_ptr<Component> Create() = 0;
};

template<typename type>
class ComponentCreatorType : public ComponentCreator
{
public:
	ComponentCreatorType(ComponentTypeId _typeId)
		: ComponentCreator(_typeId)
	{
	}

	virtual std::shared_ptr<Component> Create() override
	{
		std::shared_ptr<Component> component = std::make_shared<type>();
		component->TypeId = TypeId;
		return component;
	}
};

// Macro to register Component
#define RegisterComponent(type) FACTORY->AddComponentCreator( #type, std::make_shared<ComponentCreatorType<type>>( ComponentTypeId::CT_##type ) );