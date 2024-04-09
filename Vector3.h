/******************************************************************************/
/*
	File Name: Vector3.h
	Project Name: WhereGotLate
	Author(s): Lee Shi Lin [100%]

	Brief: Vector2 is a data type that holds 3 floats

	Copyright Information:
	All content © 2019 DigiPen (SINGAPORE) Corporation, all rights reserved.
*/
/******************************************************************************/
#pragma once
#include "ISerialization.h"

class VEC3
{
public:
	float x, y, z;
	// Constructors
	VEC3();
	VEC3(float _x, float _y, float _z);
	VEC3(const VEC3& vect);

	// Assignment operators
	VEC3& operator = (const VEC3& vect);
	VEC3& operator += (const VEC3 &rhs);
	VEC3& operator -= (const VEC3& rhs);
	VEC3& operator *= (float rhs);
	VEC3& operator /= (float rhs);

	// Unary operators
	VEC3 operator -() const;

	//Serialization
	void Serialize(ISerialization& _stream);
};

// Binary operator
VEC3 operator + (const VEC3& lhs, const VEC3& rhs);
VEC3 operator - (const VEC3& lhs, const VEC3& rhs);
VEC3 operator * (const VEC3& lhs, float rhs);
VEC3 operator * (float lhs, const VEC3& rhs);
VEC3 operator / (const VEC3& lhs, float rhs);

// Comparison operator
bool operator == (const VEC3& lhs, const VEC3& rhs);
bool operator != (const VEC3& lhs, const VEC3& rhs);

// Dot product
float Vec3Dot(const VEC3& pVec0, const VEC3& pVec1);

// Cross Product
VEC3 Vec3Cross(const VEC3& pVec0, const VEC3& pVec1);

// Lerp