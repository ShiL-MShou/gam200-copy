/******************************************************************************/
/*
	File Name: Vector2.h
	Project Name: WhereGotLate
	Author(s): Lee Shi Lin [90%]
			   Ong Cheng Kai Marcus [10%]

	Brief: Vector2 is a data type that holds 2 floats

	Copyright Information:
	All content © 2019 DigiPen (SINGAPORE) Corporation, all rights reserved.
*/
/******************************************************************************/
#pragma once
#pragma warning(disable : 4201) // nameless struct

#include "ISerialization.h"

union VEC2
{	
	struct 
	{
		float x, y;
	};

	float vec[2];
	// Constructors
	VEC2();
	VEC2(float _x, float _y);
	VEC2(const VEC2& vect);

	// Assignment operators
	VEC2& operator = (const VEC2& vect);
	VEC2& operator += (const VEC2& rhs);
	VEC2& operator -= (const VEC2& rhs);
	VEC2& operator *= (float rhs);
	VEC2& operator /= (float rhs);

	// Unary operators
	VEC2 operator -() const;

	//serialization
	void Serialize(ISerialization& _stream);

	// For debugging
	void printVEC2();

	// Get x
	float getx();
	// Get y
	float gety();
	
};

// Binary operator
VEC2 operator + (const VEC2& lhs, const VEC2& rhs);
VEC2 operator - (const VEC2& lhs, const VEC2& rhs);
VEC2 operator * (const VEC2& lhs, float rhs);
VEC2 operator * (float lhs, const VEC2& rhs);
VEC2 operator / (const VEC2& lhs, float rhs);

// Comparison operator
bool operator == (const VEC2& lhs, const VEC2& rhs);
bool operator != (const VEC2& lhs, const VEC2& rhs);

// Dot product
float Vec2Dot(const VEC2& pVec0, const VEC2& pVec1);

//normalize
void VEC2Normalize(VEC2& pResult, const VEC2& pVec0);

//length/magnitude
float VEC2Length(const VEC2& pVec0);

float VEC2SquaredLength(const VEC2& pVec0);

float VEC2Magnitude(const VEC2& pVec0, const VEC2& pVec1);

VEC2 VEC2MoveTowards(VEC2 curr, VEC2 target, float MaxDistDelta);

// Distance between 2 points
float VEC2Distance(const VEC2& pVec0, const VEC2& pVec1);