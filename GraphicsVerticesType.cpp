/*************************************************************************************/
/*
File Name:		GraphicsVerticesType.cpp
Project Name:	WhereGotLate
Author(s):		Tay Qin Wen Alvin [100%]


Brief:		GraphicsVertices is format used for the vertex buffer.


Copyright Information:
All content © 2019 DigiPen (SINGAPORE) Corporation, all rights reserved.
*/
/*************************************************************************************/

#include "stdafx.h"
#include "GraphicsVerticesType.h"


GraphicsVertexType::GraphicsVertexType()
	: x{ 0 }, y{ 0 }, z{ 0 }, u{ 0 }, v{ 0 }
{
}

GraphicsVertexType::GraphicsVertexType(const float* pArr)
	: x{ pArr[0] }, y{ pArr[1] }, z{ pArr[2] }, u{ pArr[3] }, v{ pArr[4] }
{
}

GraphicsVertexType::GraphicsVertexType(const GraphicsVertexType& rhs)
	: x{ rhs.x }, y{ rhs.y }, z{ rhs.z }, u{ rhs.u }, v{ rhs.v }
{
}

GraphicsVertexType::GraphicsVertexType(float _x, float _y, float _z, float _u, float _v)
	: x{ _x }, y{ _y }, z{ _z }, u{ _u }, v{ _v }
{
}

GraphicsVertexType& GraphicsVertexType::operator=(const GraphicsVertexType& rhs)
{
	x = rhs.x;
	y = rhs.y;
	z = rhs.z;

	u = rhs.u;
	v = rhs.v;

	return *this;
}


GraphicsVerticesType::GraphicsVerticesType()
	: vertex0{}, vertex1{}, vertex2{}, vertex3{}
{
}

GraphicsVerticesType::GraphicsVerticesType(const float* pArr)
	:	vertex0{ pArr[0],	pArr[1],	pArr[2],	pArr[3],	pArr[4] },
		vertex1{ pArr[5],	pArr[6],	pArr[7],	pArr[8],	pArr[9] },
		vertex2{ pArr[10],	pArr[11],	pArr[12],	pArr[13],	pArr[14] },
		vertex3{ pArr[15],	pArr[16],	pArr[17],	pArr[18],	pArr[19] }
{
}

GraphicsVerticesType::GraphicsVerticesType(const GraphicsVerticesType& rhs)
	: vertex0{ rhs.vertex0 }, vertex1{ rhs.vertex1 }, vertex2{ rhs.vertex2 }, vertex3{ rhs.vertex3 }
{
}

GraphicsVerticesType::GraphicsVerticesType(GraphicsVertexType _vertex0, GraphicsVertexType _vertex1, GraphicsVertexType _vertex2, GraphicsVertexType _vertex3)
	: vertex0{ _vertex0 }, vertex1{ _vertex1 }, vertex2{ _vertex2 }, vertex3{ _vertex3 }
{
}

GraphicsVerticesType::GraphicsVerticesType(	float _vertex0x, float _vertex0y, float _vertex0z, float _vertex0u, float _vertex0v,
											float _vertex1x, float _vertex1y, float _vertex1z, float _vertex1u, float _vertex1v, 
											float _vertex2x, float _vertex2y, float _vertex2z, float _vertex2u, float _vertex2v, 
											float _vertex3x, float _vertex3y, float _vertex3z, float _vertex3u, float _vertex3v)
	:	vertex0{ _vertex0x,	_vertex0y,	_vertex0z,	_vertex0u,	_vertex0v },
		vertex1{ _vertex1x,	_vertex1y,	_vertex1z,	_vertex1u,	_vertex1v },
		vertex2{ _vertex2x,	_vertex2y,	_vertex2z,	_vertex2u,	_vertex2v },
		vertex3{ _vertex3x,	_vertex3y,	_vertex3z,	_vertex3u,	_vertex3v }
{
}

GraphicsVerticesType& GraphicsVerticesType::operator=(const GraphicsVerticesType& rhs)
{
	vertex0 = rhs.vertex0;
	vertex1 = rhs.vertex1;
	vertex2 = rhs.vertex2;
	vertex3 = rhs.vertex3;

	return *this;
}