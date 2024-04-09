/******************************************************************************/
/*
	File Name: Vector3.cpp
	Project Name: WhereGotLate
	Author(s): Lee Shi Lin [100%]

	Brief: Vector2 is a data type that holds 3 floats

	Copyright Information:
	All content © 2019 DigiPen (SINGAPORE) Corporation, all rights reserved.
*/
/******************************************************************************/
#include "stdafx.h"
#include "Vector3.h"

// Constructors
VEC3::VEC3() : x{0}, y{0}, z{0} {}
VEC3::VEC3(float _x, float _y, float _z) : x{_x}, y{_y}, z{_z} {}

VEC3::VEC3(const VEC3& vect) : x{vect.x}, y{vect.y}, z{vect.z}
{
}

VEC3& VEC3::operator=(const VEC3& vect)
{
	this->x = vect.x;
	this->y = vect.y;
	this->z = vect.z;
	return *this;
}

// Assignment operators
VEC3& VEC3::operator += (const VEC3& rhs) {
	this->x = this->x + rhs.x;
	this->y = this->y + rhs.y;
	this->z = this->z + rhs.z;
	return *this;
}
VEC3& VEC3::operator -= (const VEC3& rhs) {
	this->x = this->x - rhs.x;
	this->y = this->y - rhs.y;
	this->z = this->z - rhs.z;
	return *this;
}
VEC3& VEC3::operator *= (float rhs) {
	this->x = this->x * rhs;
	this->y = this->y * rhs;
	this->z = this->z * rhs;
	return *this;
}
VEC3& VEC3::operator /= (float rhs) {
	this->x = this->x / rhs;
	this->y = this->y / rhs;
	this->z = this->z / rhs;
	return *this;
}

// Unary operators
VEC3 VEC3::operator -() const {
	return VEC3(-(this->x), -(this->y), -(this->z));
}

//Serialization
void VEC3::Serialize(ISerialization& _stream)
{
	StreamRead(_stream, x);
	StreamRead(_stream, y);
	StreamRead(_stream, z);
}

// Binary operators
VEC3 operator + (const VEC3& lhs, const VEC3& rhs) {
	return VEC3(lhs.x + rhs.x, lhs.y + rhs.y, lhs.z + rhs.z);
}
VEC3 operator - (const VEC3& lhs, const VEC3& rhs) {
	return VEC3(lhs.x - rhs.x, lhs.y - rhs.y, lhs.z - rhs.z);
}
VEC3 operator * (const VEC3& lhs, float rhs) {
	return VEC3(lhs.x * rhs, lhs.y * rhs, lhs.z * rhs);
}
VEC3 operator * (float lhs, const VEC3& rhs) {
	return VEC3(rhs.x * lhs, rhs.y * lhs, rhs.z * lhs);
}
VEC3 operator / (const VEC3& lhs, float rhs) {
	return VEC3(lhs.x / rhs, lhs.y / rhs, lhs.z / rhs);
}

bool operator==(const VEC3& lhs, const VEC3& rhs)
{
	bool cx = (lhs.x == rhs.x) ? true : false;
	bool cy = (lhs.y == rhs.y) ? true : false;
	bool cz = (lhs.z == rhs.z) ? true : false;
	if (cx && cy && cz)
		return true;

	return false;
}

bool operator!=(const VEC3& lhs, const VEC3& rhs)
{
	bool cx = (lhs.x == rhs.x) ? true : false;
	bool cy = (lhs.y == rhs.y) ? true : false;
	bool cz = (lhs.z == rhs.z) ? true : false;
	if (cx && cy && cz)
		return false;

	return true;
}

float Vec3Dot(const VEC3& pVec0, const VEC3& pVec1)
{
	return pVec0.x * pVec1.x + pVec0.y * pVec1.y + pVec0.z * pVec1.z;
}

VEC3 Vec3Cross(const VEC3& pVec0, const VEC3& pVec1)
{
	float cx = pVec0.y * pVec1.z - pVec0.z * pVec1.y;
	float cy = pVec0.z * pVec1.x - pVec0.x * pVec1.z;
	float cz = pVec0.x * pVec1.y - pVec0.y * pVec1.x;
	return VEC3(cx, cy, cz);
}