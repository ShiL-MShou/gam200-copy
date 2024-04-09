/*************************************************************************************/
/*
File Name:		Texture.h
Project Name:	WhereGotLate
Author(s):		Tay Qin Wen Alvin [100%]


Brief:			Texture holds all the information and its subtextures


Copyright Information:
All content © 2019 DigiPen (SINGAPORE) Corporation, all rights reserved.
*/
/*************************************************************************************/


#pragma once

#include <string>
#include <map>
#include <vector>
#include <array>
#include <utility>

#include "Renderer.h"
#include "Vector2.h"

// seralisation
#include "RapidjsonSerialization.h"

struct TextureFormat
{
	int		maxFrame;
	int		numCol;
	int		numRow;
	VEC2	textureCoordsDiff;
	VEC2	size;
};



class Texture
{
	int textureID;
	std::string name;

	unsigned int m_rendererID;
	int m_width, m_height;

	std::map <	std::string,
				std::unique_ptr < std::pair <	TextureFormat,
												std::vector<std::array<VEC2, 4>> >>> subTextureLst;

	inline void SerializeDiffGrid(JsonReader& _jr, const std::string& _key1);
	inline void SerializeSameGrid(JsonReader& _jr, const std::string& _key1);


public:
	Texture();
	Texture(int _width, int _height);
	Texture(const std::string& path);
	~Texture();

	// slots for texture, usually ard 32 for windows
	void Bind(unsigned int slot = 0) const;
	void Unbind() const;

	void Serialize(JsonReader& _jr, const std::string& _key1);


	void SetTextureID(int _textureID);

	// getters
	inline int GetWidth() const { return m_width; }
	inline int GetHeight() const { return m_height; }

	int GetMaxFrame(std::string _name) const;
	//unsigned int GetNumCol(std::string _name) const;
	//unsigned int GetNumRow(std::string _name) const;
	VEC2 GetSize(std::string _name) const;

	std::string GetName() const; 

	std::array<VEC2, 4> GetUVs(std::string _name, unsigned int _frameNum) const;

	unsigned int GetRenderID() { return m_rendererID; }

	friend struct Sprite;
	friend class SpriteComponent;
};

