/************************************************************************************ /
/*
File Name:		VertexArray.h
Project Name:	WhereGotLate
Author(s):		Tay Qin Wen Alvin [100%]


Brief:			Vertex Array for graphics.


Copyright Information:
All content © 2019 DigiPen (SINGAPORE) Corporation, all rights reserved.
*/
/*************************************************************************************/

#pragma once

#include "VertexBuffer.h"
#include "VertexBufferLayout.h"


class VertexArray
{
	unsigned int m_rendererID;

public:
	VertexArray();
	~VertexArray();

	VertexArray(const VertexArray& _rhs);
	VertexArray& operator=(const VertexArray& _rhs);


	void AddBuffer(const VertexBuffer& vb, const VertexBufferLayout& layout);

	void Bind() const;
	void Unbind() const;
};
