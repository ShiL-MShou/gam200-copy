/************************************************************************************ /
/*
File Name:		IndexBuffer.h
Project Name:	WhereGotLate
Author(s):		Tay Qin Wen Alvin [100%]


Brief:			Index buffer to reuse the vertices.


Copyright Information:
All content © 2019 DigiPen (SINGAPORE) Corporation, all rights reserved.
*/
/*************************************************************************************/


#include "stdafx.h"
#include "IndexBuffer.h"
#include "Renderer.h"				// for ASSERT


IndexBuffer::IndexBuffer()
	: m_RendererID{ 0 }, m_Count{ 0 }
{

}

IndexBuffer::IndexBuffer(const unsigned int* data, unsigned int count)
	: m_RendererID{ 0 }, m_Count{ count }
{
	// to be safe
	//ASSERT(sizeof(unsigned int) == sizeof(GLuint));

	glGenBuffers(1, &m_RendererID);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_RendererID);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * sizeof(unsigned int), data, GL_STATIC_DRAW);

}


IndexBuffer::~IndexBuffer()
{
	glDeleteBuffers(1, &m_RendererID);
}

void IndexBuffer::Bind() const
{
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_RendererID);
}

void IndexBuffer::Unbind() const
{
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

