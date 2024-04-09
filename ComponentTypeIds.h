/*************************************************************************************/
/*
File Name:		ComponentTypeIds.h
Project Name:	WhereGotLate
Author(s):		Kok Zhe Yan			 [100%]


Copyright Information:
All content © 2019 DigiPen (SINGAPORE) Corporation, all rights reserved.
*/
/*************************************************************************************/
#pragma once

enum class ComponentTypeId
{
	// Invalid component id
	CT_None = 0,
	// Basic component
	CT_Transform,	
	// Graphics
	CT_Camera,
	CT_SpriteComponent,
	//CT_Animator,
	CT_AnimationComponent,
	// Physics
	CT_Body,
	CT_BoxCollider,
	// Logic
	CT_Controller,
	CT_LogicComponent,

	CT_MaxComponents
	
};