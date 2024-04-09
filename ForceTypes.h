/*****************************************************************************/
/*
File Name:		ForceTypes.h
Project Name:	WhereGotLate
Author(s):		Ong Cheng Kai Marcus [100%]

Copyright information:
All content copyright 2020 DigiPen (SINGAPORE) Corporation, all rights reserved.
*/
/*****************************************************************************/
#pragma once
enum class FORCE_TYPE
{
	LINEAR = 0,
	DRAG = 1,
	NUM_TYPES
};

enum ForceVecMember
{
	LEFT = 0,
	RIGHT = 1,
	UP = 2,
	DOWN = 3,
	DRAGFORCE = 4,
	DIAGONALUPLEFT,
	DIAGONALUPRIGHT,
	GRAVITY,
	NUM_FORCES
};