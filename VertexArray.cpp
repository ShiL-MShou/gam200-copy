/************************************************************************************ /
/*
File Name:		VertexArray.cpp
Project Name:	WhereGotLate
Author(s):		Tay Qin Wen Alvin [100%]


Brief:			Vertex Array for graphics.


Copyright Information:
All content © 2019 DigiPen (SINGAPORE) Corporation, all rights reserved.
*/
/*************************************************************************************/


#include "stdafx.h"
#include "VertexArray.h"
#include "Renderer.h"

#include "Graphics.h"


VertexArray::VertexArray()
{
	glGenVertexArrays(1, &m_rendererID);
}

VertexArray::~VertexArray()
{
	glDeleteVertexArrays(1, &m_rendererID);
}

VertexArray::VertexArray(const VertexArray& _rhs)
	: m_rendererID { _rhs.m_rendererID }
{
}

VertexArray& VertexArray::operator=(const VertexArray& _rhs)
{
	m_rendererID = _rhs.m_rendererID;

	return *this;
}

void VertexArray::AddBuffer(const VertexBuffer& vb, const VertexBufferLayout& layout)
{
	Bind();
	vb.Bind();

	// linking buffer and array
	const auto& elements = layout.GetElements();
	long long offset = 0;

	
	for (int i = 0; i < elements.size(); ++i)
	{
		const VertexBufferElement& element = elements[i];

		// tell opengl how to read the data.
		glEnableVertexAttribArray(i);
		glVertexAttribPointer(i, element.count, element.type, element.normalised, layout.GetStride(), (const void*)offset /* (const void*)8 for 1  or use offsetof */);

		offset += static_cast<long long>(element.count) * VertexBufferElement::GetSizeOfType(element.type);

		//glVertexAttribDivisor(i, 1);
	}

	{
		glBindBuffer(GL_ARRAY_BUFFER, GRAPHICS->buffer);
		glBufferData(GL_ARRAY_BUFFER, INSTANCELIMIT2 * sizeof(glm::mat4), &(*GRAPHICS->GetModelMatrices())[0], GL_STATIC_DRAW);

		offset = 0;

		glEnableVertexAttribArray(2);
		glVertexAttribPointer(2, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)offset);
		glVertexAttribDivisor(2, 1);

		offset += sizeof(glm::mat4);

		glEnableVertexAttribArray(3);
		glVertexAttribPointer(3, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)offset);
		glVertexAttribDivisor(3, 1);

		offset += sizeof(glm::mat4);

		glEnableVertexAttribArray(4);
		glVertexAttribPointer(4, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)offset);
		glVertexAttribDivisor(4, 1);

		offset += sizeof(glm::mat4);

		glEnableVertexAttribArray(5);
		glVertexAttribPointer(5, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)offset);
		glVertexAttribDivisor(5, 1);

		offset += sizeof(glm::mat4);
	}
}

void VertexArray::Bind() const
{
	glBindVertexArray(m_rendererID);
}

void VertexArray::Unbind() const
{
	glBindVertexArray(0);
}
