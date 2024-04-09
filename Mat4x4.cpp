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
#include "stdafx.h"

#include <iostream>
#include "Mat4x4.h"
#include <cmath>


void MAT4::PrintMat4()
{
	for (int i = 0; i < 4; i++)
		for (int j = 0; j < 4; j++)
			std::cout << M[i][j] << (j == 3 ? '\n' : ' ');
	std::cout << std::endl;
}

MAT4::MAT4()
	: M{}
{
	for (int i = 0; i < 4; i++) {
		for (int k = 0; k < 4; k++) {
			M[i][k] = i == k ? 1.0f : 0.0f;
		}
	}
}

MAT4 MAT4::Scale(const float x, const float y, const float z)
{
	MAT4 S;

	S.M[0][0] = x;  /* x 0 0 0 */
	S.M[1][1] = y;  /* 0 y 0 0 */
	S.M[2][2] = z;  /* 0 0 z 0 */
					/* 0 0 0 1 */
	return S;
}

MAT4 MAT4::Translate(const float x, const float y, const float z)
{
	MAT4 T; /* Identity Matrix */

	T.M[0][3] = x;  /* 1 0 0 x */
	T.M[1][3] = y;  /* 0 1 0 y */
	T.M[2][3] = z;  /* 0 0 1 z */
					/* 0 0 0 1 */
	return T;
}
// Returns a rotation matrix around an axis(0:X, 1 : Y, 2 : Z)
MAT4 MAT4::Rotate(const int i, const float theta)
{
	MAT4 R;
	float PI = 3.1415926535897f;
	/* Convert theta to radian */
	float Rad = theta * PI / 180;

	switch (i)
	{
	case 0: /* x-axis */
		R.M[1][1] = cosf(Rad);   /* 1  0    0  0 */
		R.M[1][2] = -sinf(Rad);  /* 0 cos -sin 0 */
		R.M[2][1] = sinf(Rad);   /* 0 sin  cos 0 */
		R.M[2][2] = cosf(Rad);   /* 0  0    0  1 */
		break;
	case 1: /* y-axis */
		R.M[0][0] = cosf(Rad);   /*  cos 0 sin 0 */
		R.M[2][0] = -sinf(Rad);  /*   0  1  0  0 */
		R.M[0][2] = sinf(Rad);   /* -sin 0 cos 0 */
		R.M[2][2] = cosf(Rad);   /*   0  0  0  1 */
		break;
	case 2: /* z-axis */
		R.M[0][0] = cosf(Rad);   /* cos -sin 0 0 */
		R.M[1][0] = sinf(Rad);   /* sin  cos 0 0 */
		R.M[0][1] = -sinf(Rad);  /*  0    0  1 0 */
		R.M[1][1] = cosf(Rad);   /*  0    0  0 1 */
		break;
	}

	return R;
}

void MAT4::Transpose(MAT4& _pResult)
{
	/*MAT4 transpose;

	for (int i = 0; i < 4; i++){
		for (int k = 0; k < 4; k++) {
			transpose.M[k][i] = matrix.M[i][k];
		}
	}
	return transpose;*/

	for (int i = 0; i < 4; i++) {
		for (int k = 0; k < 4; k++) {
			_pResult.M[k][i] = M[i][k];
		}
	}
}

double Sin_Rad(double radangle)
{
	return sin(radangle);
}

double Cos_Rad(double radangle)
{
	return cos(radangle);
}

double Tan_Rad(double radangle)
{
	return tan(radangle);
}
/*
double Sin_Deg(double degangle)
{
	
	return sin(degangle);
}

double Cos_Deg(double degangle)
{
	
	return cos(degangle);
}

double Tan_Deg(double degangle)
{
	
	return tan(degangle);
}
*/