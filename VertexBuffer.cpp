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


#include "stdafx.h"
#include "VertexBuffer.h"

#include "Graphics.h"

VertexBuffer::VertexBuffer()
	: m_RendererID{ 0 }, m_data{}
{

}

// not tested
VertexBuffer::VertexBuffer(GraphicsVerticesType _data)
	: m_data{_data}
{
	// generate buffer
	glGenBuffers(1, &m_RendererID);
	// bind buffer and set data
	glBindBuffer(GL_ARRAY_BUFFER, m_RendererID);
	//glBufferData(GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW);
	glBufferData(GL_ARRAY_BUFFER, sizeof(m_data), &m_data, GL_STATIC_DRAW);
}

VertexBuffer::VertexBuffer(void* data, unsigned int size)
{
	// generate buffer
	glGenBuffers(1, &m_RendererID);
	// bind buffer and set data
	glBindBuffer(GL_ARRAY_BUFFER, m_RendererID);
	//glBufferData(GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW);
	glBufferData(GL_ARRAY_BUFFER, (unsigned int)(10 * size), data, GL_STATIC_DRAW);
}


VertexBuffer::~VertexBuffer()
{
	glDeleteBuffers(1, &m_RendererID);
}

VertexBuffer::VertexBuffer(const VertexBuffer& _rhs)
	: m_RendererID	{_rhs.m_RendererID}
{
}

VertexBuffer& VertexBuffer::operator=(const VertexBuffer& _rhs)
{
	// TODO: insert return statement here???
	m_RendererID = _rhs.m_RendererID;

	return *this;
}

void VertexBuffer::SetAlpha(float _a)
{
	_a;
	//m_data.vertex0.a = _a;
	//m_data.vertex1.a = _a;
	//m_data.vertex2.a = _a;
	//m_data.vertex3.a = _a;
}

void VertexBuffer::SetDepth(float _z)
{
	m_data.vertex0.z = _z;
	m_data.vertex1.z = _z;
	m_data.vertex2.z = _z;
	m_data.vertex3.z = _z;
}

void VertexBuffer::SetUV(std::array<VEC2, 4> _uv)
{
	m_data.vertex0.u = _uv[0].x;
	m_data.vertex0.v = _uv[0].y;

	m_data.vertex1.u = _uv[1].x;
	m_data.vertex1.v = _uv[1].y;

	m_data.vertex2.u = _uv[2].x;
	m_data.vertex2.v = _uv[2].y;

	m_data.vertex3.u = _uv[3].x;
	m_data.vertex3.v = _uv[3].y;
}

void VertexBuffer::UpdateData()
{
	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(m_data), &m_data);
}

void VertexBuffer::Bind() const
{
	glBindBuffer(GL_ARRAY_BUFFER, m_RendererID);
}

void VertexBuffer::Unbind() const
{
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}
