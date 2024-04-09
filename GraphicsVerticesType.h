/*************************************************************************************/
/*
File Name:		GraphicsVerticesType.h
Project Name:	WhereGotLate
Author(s):		Tay Qin Wen Alvin [100%]


Brief:		GraphicsVertices is format used for the vertex buffer.


Copyright Information:
All content © 2019 DigiPen (SINGAPORE) Corporation, all rights reserved.
*/
/*************************************************************************************/

#pragma once


#ifdef _MSC_VER
// Supress warning: nonstandard extension used : nameless struct/union
#pragma warning( disable : 4201 )
#endif


//#include "GraphicsVertexType.h"

#define GVT_ROWS 4
#define GVT_COLS 5


typedef union GraphicsVertexType
{
	struct
	{
		float x, y, z, u, v;
	};

	float m[GVT_COLS];

	GraphicsVertexType();

	GraphicsVertexType(const float* pArr);

	GraphicsVertexType(const GraphicsVertexType& rhs);

	GraphicsVertexType(float _x, float _y, float _z, float _u, float _v);

	GraphicsVertexType& operator=(const GraphicsVertexType& rhs);
} GraphicsVertexType;


typedef union GraphicsVerticesType
{
	struct
	{
		GraphicsVertexType vertex0, vertex1, vertex2, vertex3;
	};

	float m1[GVT_ROWS * GVT_COLS];
	float m2[GVT_ROWS][GVT_COLS];

	// ----------------------------------------------------------------------------------------------------------------------------

	GraphicsVerticesType();

	GraphicsVerticesType(	const float* pArr);
	
	GraphicsVerticesType(	const GraphicsVerticesType& rhs);

	GraphicsVerticesType(	GraphicsVertexType _vertex0, GraphicsVertexType _vertex1, GraphicsVertexType _vertex2, GraphicsVertexType _vertex3);

	GraphicsVerticesType(	float _vertex0x, float _vertex0y, float _vertex0z, float _vertex0u, float _vertex0v,
							float _vertex1x, float _vertex1y, float _vertex1z, float _vertex1u, float _vertex1v,
							float _vertex2x, float _vertex2y, float _vertex2z, float _vertex2u, float _vertex2v,
							float _vertex3x, float _vertex3y, float _vertex3z, float _vertex3u, float _vertex3v );

	
	GraphicsVerticesType& operator=(const GraphicsVerticesType& rhs);

} GraphicsVerticesType, gvType;

