/*************************************************************************************/
/*
File Name:		VertexBufferLayout.h
Project Name:	WhereGotLate
Author(s):		Tay Qin Wen Alvin [100%]


Brief:			This tells the graphics the layout of the VertexBuffer.


Copyright Information:
All content © 2019 DigiPen (SINGAPORE) Corporation, all rights reserved.
*/
/*************************************************************************************/

#pragma once

#include <vector>
#include <glew.h>


struct VertexBufferElement
{
	unsigned int type;
	unsigned int count;
	bool normalised;

	static unsigned int GetSizeOfType(unsigned int type);
};


class VertexBufferLayout
{
	std::vector<VertexBufferElement> m_elements;
	unsigned int m_stride;								// size till next

public:
	VertexBufferLayout();

	~VertexBufferLayout();

	template <typename T>
	void Push(unsigned int count)
	{
		static_assert(false);
	}

	template<>
	void Push<float>(unsigned int count)
	{
		m_elements.push_back(VertexBufferElement{ GL_FLOAT, count, GL_FALSE });
		m_stride += VertexBufferElement::GetSizeOfType(GL_FLOAT) * count;
	}


	template<>
	void Push<unsigned int>(unsigned int count)
	{
		m_elements.push_back(VertexBufferElement{ GL_UNSIGNED_INT, count, GL_FALSE });
		m_stride += VertexBufferElement::GetSizeOfType(GL_UNSIGNED_INT) * count;
	}


	template<>
	void Push<unsigned char>(unsigned int count)
	{
		m_elements.push_back(VertexBufferElement{ GL_UNSIGNED_BYTE, count, GL_TRUE });
		m_stride += VertexBufferElement::GetSizeOfType(GL_UNSIGNED_BYTE) * count;
	}

	// Gets
	inline const std::vector<VertexBufferElement>& GetElements() const { return m_elements; }
	inline unsigned int GetStride() const { return m_stride; }
};

