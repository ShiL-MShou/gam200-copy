/*************************************************************************************/
/*
File Name:		TextRenderer.h
Project Name:	WhereGotLate
Author(s):		Tay Qin Wen Alvin [100%]


Brief:			TextRenderer handles all the text and renders them.


Copyright Information:
All content © 2019 DigiPen (SINGAPORE) Corporation, all rights reserved.
*/
/*************************************************************************************/


#pragma once

#include <map>
#include <memory>
#include <string>
#include <vector>

#include "VertexArray.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "Shader.h"

#include "Vector2.h"
#include "Vector3.h"

#define TextShader (MyShaderManager.GetShader(ShaderType::SHADER_TXT))


/// Holds all state information relevant to a character as loaded using FreeType
struct Character {

	unsigned int	TextureID = 0;		// ID handle of the glyph texture
	VEC2			Size;			// Size of glyph
	VEC2			Bearing;		// Offset from baseline to left/top of glyph
	unsigned int	Advance = 0;		// Horizontal offset to advance to next glyph
};

struct TextFormat
{

	std::string		text;
	VEC2			pos;
	float			size = 0.0f;
	VEC3			color;

	TextFormat(std::string _text, VEC2 _pos, float _size, VEC3 _color);
};


class TextRenderer
{
	std::map<char, Character>		Characters;

	std::unique_ptr<VertexArray>	vertexArrayPtr;
	std::unique_ptr<VertexBuffer>	vertexBufferPtr;
	std::unique_ptr<IndexBuffer>	indexBufferPtr;

	float							depth;

	std::vector<TextFormat>			textList;

public:
	TextRenderer();
	~TextRenderer();

	TextRenderer(const TextRenderer&) = delete;
	TextRenderer& operator=(const TextRenderer&) = delete;

	void Initialize(std::shared_ptr<Shader> _shaderPtr);

	void RenderText(glm::mat4 _vp);

	void AddText(std::string _text, VEC2 _pos, float _size = 0.75f, VEC3 _color = VEC3(0.4f, 0.4f, 0.4f));

};

