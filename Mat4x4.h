/******************************************************************************/
/*
	File Name: Mat4x4.h
	Project Name: WhereGotLate
	Author(s): Lee Shi Lin [100%]

	Brief: Mat4x4 is a data type that holds a 4 by 4 matrix of floats

	Copyright Information:
	All content © 2019 DigiPen (SINGAPORE) Corporation, all rights reserved.
*/
/******************************************************************************/
#pragma once

#include <array>
#include "Vector2.h"
#include "Vector3.h"

typedef float ROW4[4]; // For accessing rows

/* Important classes - MAT4, Vec3, Vec4 */
union MAT4
{
	struct
	{
		float m00, m01, m02, m03;
		float m10, m11, m12, m13;
		float m20, m21, m22, m23;
		float m30, m31, m32, m33;
	};
	/*union
	{
		std::array<float, 16> ma;
		float m[16];
		float M[4][4];
	};*/

	std::array<float, 16> ma;
	float m[16];
	float M[4][4];

	// Initialize to identity matrix
	MAT4();
	// Copy constructor

	// Get index operation - get access to row address
	ROW4& operator[](const int i) { return M[i]; }
	const ROW4& operator[](const int i) const { return M[i]; }

	///////////////////////
	/* TYPES OF MATRICES */
	///////////////////////
	// Scale
	MAT4 Scale(const float x, const float y, const float z);
	// Translate
	MAT4 Translate(const float x, const float y, const float z);
	// Rotate - (0:X, 1 : Y, 2 : Z)
	MAT4 Rotate(const int i, const float theta);

	// Transpose
	void Transpose(MAT4& _pResult);

	// For Debugging - print 4x4 matrix
	void PrintMat4();
};

/******************************************************************************/
/*!
	\brief
	Sin, Cos, Tan function.

	\param angle
	The angle in RADIAN
*/
/******************************************************************************/
double Sin_Rad(double radangle);

double Cos_Rad(double radangle);

double Tan_Rad(double radangle);

/******************************************************************************/
/*!
	\brief
	Sin, Cos, Tan function.

	\param angle
	The angle in DEGREE
*/
/******************************************************************************/
/*
double Sin_Deg(double degangle);

double Cos_Deg(double degangle);

double Tan_Deg(double degangle);
*/