/************************************************************************************ /
/*
File Name:		VertexBuffer.h
Project Name:	WhereGotLate
Author(s):		Tay Qin Wen Alvin [100%]


Brief:			Vertex Buffer for graphics


Copyright Information:
All content © 2019 DigiPen (SINGAPORE) Corporation, all rights reserved.
*/
/*************************************************************************************/


#pragma once

#include <array>

#include "GraphicsVerticesType.h"
#include "Vector2.h"


class VertexBuffer
{
	unsigned int m_RendererID;
	GraphicsVerticesType m_data;

public:
	VertexBuffer();
	VertexBuffer(void* data, unsigned int size);
	VertexBuffer(GraphicsVerticesType _data);
	~VertexBuffer();


	VertexBuffer(const VertexBuffer& _rhs);
	VertexBuffer& operator=(const VertexBuffer& _rhs);

	void SetAlpha(float _a);
	void SetDepth(float _z);
	void SetUV(std::array<VEC2, 4> _uv);
	void UpdateData();

	void Bind() const;
	void Unbind() const;
};

