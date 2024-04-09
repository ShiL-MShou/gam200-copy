/******************************************************************************/
/*
	File Name: Vector2.cpp
	Project Name: WhereGotLate
	Author(s): Lee Shi Lin [90%]
			   Ong Cheng Kai Marcus [10%]

	Brief: Vector2 is a data type that holds 2 floats

	Copyright Information:
	All content © 2019 DigiPen (SINGAPORE) Corporation, all rights reserved.
*/
/******************************************************************************/
#include "stdafx.h"
#include "Vector2.h"
#include <cmath>

// Constructors
VEC2::VEC2() : x{ 0 }, y{ 0 } {}
VEC2::VEC2(float _x, float _y) : x{ _x }, y{ _y } {}

VEC2::VEC2(const VEC2& vect) : x{vect.x}, y{vect.y}
{
}

// Assignment operators
VEC2& VEC2::operator=(const VEC2& vect)
{
	this->x = vect.x;
	this->y = vect.y;
	return *this;
}

VEC2& VEC2::operator += (const VEC2& rhs) {
	this->x = this->x + rhs.x;
	this->y = this->y + rhs.y;
	return *this;
}

VEC2& VEC2::operator -= (const VEC2& rhs) {
	this->x = this->x - rhs.x;
	this->y = this->y - rhs.y;
	return *this;
}
VEC2& VEC2::operator *= (float rhs) {
	this->x = this->x * rhs;
	this->y = this->y * rhs;
	return *this;
}
VEC2& VEC2::operator /= (float rhs) {
	this->x = this->x / rhs;
	this->y = this->y / rhs;
	return *this;
}

// Unary operators
VEC2 VEC2::operator -() const {
	return VEC2(-(this->x), -(this->y));
}

void VEC2::Serialize(ISerialization& _stream)
{
	StreamRead(_stream, x);
	StreamRead(_stream, y);
}

void VEC2::printVEC2()
{
	std::cout << "Vector 2: X: " << this->x << " Y: " << this->y << std::endl;
}

float VEC2::getx()
{
	return x;
}

float VEC2::gety()
{
	return y;
}

// Binary operators
VEC2 operator + (const VEC2& lhs, const VEC2& rhs) {
	return VEC2(lhs.x + rhs.x, lhs.y + rhs.y);
}
VEC2 operator - (const VEC2& lhs, const VEC2& rhs) {
	return VEC2(lhs.x - rhs.x, lhs.y - rhs.y);
}
VEC2 operator * (const VEC2& lhs, float rhs) {
	return VEC2(lhs.x * rhs, lhs.y * rhs);
}
VEC2 operator * (float lhs, const VEC2& rhs) {
	return VEC2(rhs.x * lhs, rhs.y * lhs);
}
VEC2 operator / (const VEC2& lhs, float rhs) {
	return VEC2(lhs.x / rhs, lhs.y / rhs);
}

bool operator==(const VEC2& lhs, const VEC2& rhs)
{
	bool cx = (lhs.x == rhs.x) ? true : false;
	bool cy = (lhs.y == rhs.y) ? true : false;
	if (cx && cy)
		return true;

	return false;
}

bool operator!=(const VEC2& lhs, const VEC2& rhs)
{
	bool cx = (lhs.x == rhs.x) ? true : false;
	bool cy = (lhs.y == rhs.y) ? true : false;
	if (cx && cy)
		return false;

	return true;
}


float Vec2Dot(const VEC2& pVec0, const VEC2& pVec1)
{
	return pVec0.x * pVec1.x + pVec0.y * pVec1.y;
}

void VEC2Normalize(VEC2& pResult, const VEC2& pVec0)
{
	pResult = { pVec0.x, pVec0.y };

	if (pResult.x == 0 && pResult.y == 0)
		return;

	//formula: normal = square root( x^2 + y^2 )
	float normal = sqrtf(pResult.x * pResult.x + pResult.y * pResult.y);

	if (normal == 0)
	{
		//pResult.x = 0;
		//pResult.y = 0;

		//std::cout << "normal: "<< normal << std::endl;
		return;
	}

	//normalised coords = coord/normal
	pResult.x /= normal;
	pResult.y /= normal;
}

float VEC2Length(const VEC2& pVec0)
{
	//standard pythagoras
	return sqrt(pVec0.x * pVec0.x + pVec0.y * pVec0.y);
}

float VEC2SquaredLength(const VEC2& pVec0)
{
	return(pVec0.x * pVec0.x + pVec0.y * pVec0.y);
}

float VEC2Magnitude(const VEC2& pVec0, const VEC2& pVec1)
{
	float dx = (pVec1.x - pVec0.x) * (pVec1.x - pVec0.x);
	float dy = (pVec1.y - pVec0.y) * (pVec1.y - pVec0.y);
	return std::sqrtf(dx + dy);
}

VEC2 VEC2MoveTowards(VEC2 curr, VEC2 target, float MaxDistDelta)
{
	VEC2 dist = target - curr;

	VEC2Normalize(dist, dist);

	//std::cout << dist.x << "|" << dist.y << std::endl;

	float mag = VEC2Magnitude(curr, target);
	//std::cout << "mag: " << mag << std::endl;
	if (mag <= MaxDistDelta || mag == 0.0f)
	{
		return target;
	}

	return curr + (dist ) * MaxDistDelta;
}

float VEC2Distance(const VEC2& pVec0, const VEC2& pVec1)
{
	// Distance = Square root of ((x2-x1)^2 + (y2-y1)^2)
	float dx = (pVec1.x - pVec0.x) * (pVec1.x - pVec0.x);
	float dy = (pVec1.y - pVec0.y) * (pVec1.y - pVec0.y);
	return sqrtf(dx + dy);
}
