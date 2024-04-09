/*************************************************************************************/
/*
File Name:		TextRenderer.cpp
Project Name:	WhereGotLate
Author(s):		Tay Qin Wen Alvin [100%]


Brief:			TextRenderer handles all the text and renders them.


Copyright Information:
All content © 2019 DigiPen (SINGAPORE) Corporation, all rights reserved.
*/
/*************************************************************************************/


#include "stdafx.h"
#include "TextRenderer.h"

// --- glm ---
#include "glm.hpp"
#include "gtc/matrix_transform.hpp"
#include "gtc/type_ptr.hpp"

#include "Transform.h"

// --- freetype ---
#include <ft2build.h>
#include FT_FREETYPE_H

#include "GraphicsVerticesType.h"
#include "ShaderManager.h"

#include "CoreEngine.h"

TextRenderer::TextRenderer()
	: depth{-1.91f}
{
}

TextRenderer::~TextRenderer()
{
}


void TextRenderer::Initialize(std::shared_ptr<Shader> _shaderPtr)
{
	UNREFERENCED_PARAMETER(_shaderPtr);

	//shaderPtr = _shaderPtr;

	FT_Library ft;

	if (FT_Init_FreeType(&ft))
	{
		assert(0);	// force assert
		std::cout << "Error: Could not init FreeType Library" << std::endl;
	}

	// Load font as face
	FT_Face face;
	std::string path = CORE->mPath + "Fonts/WhereGotLateFont-Regular.ttf";
	if (FT_New_Face(ft, path.c_str(), 0, &face))
	{
		assert(0);	// force assert
		std::cout << "Error: Font failed to load" << std::endl;
	}

	// Set size to load glyphs as
	FT_Set_Pixel_Sizes(face, 0, 48);

	// Disable byte-alignment restriction
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

	// Load first 128 characters of ASCII set
	for (GLubyte c = 0; c < 128; c++)
	{
		// Load character glyph 
		if (FT_Load_Char(face, c, FT_LOAD_RENDER))
		{
			std::cout << "Error: failed to load Glyph" << std::endl;
			continue;
		}

		unsigned int texture;
		glGenTextures(1, &texture);
		glBindTexture(GL_TEXTURE_2D, texture);
		glTexImage2D(
			GL_TEXTURE_2D,
			0,
			GL_RED,
			face->glyph->bitmap.width,
			face->glyph->bitmap.rows,
			0,
			GL_RED,
			GL_UNSIGNED_BYTE,
			face->glyph->bitmap.buffer
		);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		Character character = {
			texture,
			VEC2(static_cast<float>(face->glyph->bitmap.width), static_cast<float>(face->glyph->bitmap.rows)),
			VEC2(static_cast<float>(face->glyph->bitmap_left), static_cast<float>(face->glyph->bitmap_top)),
			static_cast<unsigned int>(face->glyph->advance.x)
		};
		Characters.insert(std::pair<char, Character>(c, character));
	}
	glBindTexture(GL_TEXTURE_2D, 0);

	// Cleanup FreeType
	FT_Done_Face(face);
	FT_Done_FreeType(ft);


	GraphicsVerticesType vertices = {
		-0.5f, -0.5f, depth, 0.0f, 1.0f,	// 0
		 0.5f, -0.5f, depth, 1.0f, 1.0f,	// 1
		 0.5f,  0.5f, depth, 1.0f, 0.0f,	// 2
		-0.5f,  0.5f, depth, 0.0f, 0.0f		// 3
	};

	unsigned int indices[] = {
		0, 1, 2,
		0, 2, 3 // 2, 3, 0
	};

	// -----------------------------------------------------------------------------------------
	vertexArrayPtr = std::make_unique<VertexArray>();

	vertexBufferPtr = std::make_unique<VertexBuffer>(vertices);

	// "add" vertexBuffer into vertexArray
	VertexBufferLayout layout;
	layout.Push<float>(3);		// vertex/ length
	layout.Push<float>(2);		// texture coords
	vertexArrayPtr->AddBuffer(*vertexBufferPtr, layout);

	indexBufferPtr = std::make_unique<IndexBuffer>(indices, 6);
}


void TextRenderer::RenderText(glm::mat4 _vp)
{
	UNREFERENCED_PARAMETER(_vp);

	VEC3 tmpTextColor;

	TextShader->Bind();
	vertexArrayPtr->Bind();
	indexBufferPtr->Bind();

	
	for (auto& tmpTextFormat : textList)
	{
		// compare color
		if (tmpTextColor != tmpTextFormat.color)
		{
			// update color for text
			tmpTextColor = tmpTextFormat.color;
			TextShader->SetUniform3f("u_Color", tmpTextColor.x,
												tmpTextColor.y,
												tmpTextColor.z);
		}
		
		std::string::const_iterator c;
		for (c = tmpTextFormat.text.begin(); c != tmpTextFormat.text.end(); ++c)
		{
			Character ch = Characters[*c];

			float xpos = tmpTextFormat.pos.x + ch.Bearing.x * tmpTextFormat.size;
			float ypos = tmpTextFormat.pos.y - (ch.Size.y - ch.Bearing.y) * tmpTextFormat.size;

			float w = ch.Size.x * tmpTextFormat.size;
			float h = ch.Size.y * tmpTextFormat.size;

			float vertices2[20] = {
				 xpos,     ypos + h, depth,   0.0, 0.0,
				 xpos,     ypos,     depth,   0.0, 1.0,
				 xpos + w, ypos,     depth,   1.0, 1.0,
				 xpos + w, ypos + h, depth,   1.0, 0.0
			};

			// bind texture
			glBindTexture(GL_TEXTURE_2D, ch.TextureID);

			// update vertex buffer
			vertexBufferPtr->Bind();
			glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices2), vertices2);
			
			int num = 1;

			for (int i = 0; i < num; ++i)
			{
				TextShader->SetUniformMat4f(std::string("u_MVPArr[" + std::to_string(i) + "]"), _vp);
			}

			tmpTextFormat.pos.x += (ch.Advance >> 6) * tmpTextFormat.size;

			glDrawElementsInstanced(GL_TRIANGLES, 8, GL_UNSIGNED_INT, nullptr, num);
		}
	}

	textList.clear();
}

void TextRenderer::AddText(std::string _text, VEC2 _pos, float _size, VEC3 _color)
{
	textList.push_back(TextFormat(_text, _pos, _size, _color));
}

TextFormat::TextFormat(std::string _text, VEC2 _pos, float _size, VEC3 _color)
	: text{ _text }, pos{ _pos }, size{ _size }, color{ _color }
{
}
