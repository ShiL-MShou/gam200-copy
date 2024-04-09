/*************************************************************************************/
/*
File Name:		Texture.cpp
Project Name:	WhereGotLate
Author(s):		Tay Qin Wen Alvin [100%]


Brief:			Texture holds all the information and its subtextures


Copyright Information:
All content © 2019 DigiPen (SINGAPORE) Corporation, all rights reserved.
*/
/*************************************************************************************/


#include "stdafx.h"
#include "Texture.h"

#include "Graphics.h"
#include "SpriteList.h"
#include "Animation.h"

// to load in sprites
#include "SOIL.h"


// --- private ---

inline void Texture::SerializeDiffGrid(JsonReader& _jr, const std::string& _key1)
{
	// ---------------------------
	//	SubNum
	//	-------------
	//	SubName
	//	SubWidth
	//	SubHeight
	//  SubMaxFrame
	//	SubNumCol
	//	SubNumRow
	// ---------------------------

	// ---------------------------
	//	SubNum
	//	-------------
	//	SubName
	//	SubGridWidth
	//	SubGridHeight
	//	SubMaxColPerRow
	//	SubFrames
	// ---------------------------
	
	std::string textureName;
	_jr.ReadStringMember(_key1, "TextureName", textureName);


	int numOfSubs = 1;

	std::string subName;
	float gridWidth, gridHeight;
	
	int maxFramePerRow = 1;
	int numOfFrames = 1;

	_jr.ReadIntMember(_key1, "SubNum", numOfSubs);

	VEC2 tmpStartingPt{ 0, 0 };

	for (int i = 1; i <= numOfSubs; ++i)
	{
		_jr.ReadStringMember(	_key1, "SubName" + std::to_string(i),			subName);
		_jr.ReadFloatMember(	_key1, "SubGridWidth" + std::to_string(i),		gridWidth);
		_jr.ReadFloatMember(	_key1, "SubGridHeight" + std::to_string(i),		gridHeight);
		_jr.ReadIntMember(		_key1, "SubMaxFramePerRow" + std::to_string(i), maxFramePerRow);
		_jr.ReadIntMember(		_key1, "SubFrames" + std::to_string(i),			numOfFrames);

		VEC2 diff = { gridWidth / static_cast<float>(m_width),
					  gridHeight / static_cast<float>(m_height) };


		// --- SpriteList ---
		std::shared_ptr<SpriteList> spriteListPtr = std::make_shared<SpriteList>();

		spriteListPtr->name = subName;
		spriteListPtr->textureName = textureName;
		spriteListPtr->size = VEC2(gridWidth, gridHeight);
		// ------------------

		std::array<VEC2, 4> vec2Arr;

		while (numOfFrames > 0)
		{
			for (int j = 0; j < maxFramePerRow; ++j)
			{
				vec2Arr[0] = { diff.x * j,			tmpStartingPt.y + diff.y };
				vec2Arr[1] = { diff.x * (j + 1),	tmpStartingPt.y + diff.y };
				vec2Arr[2] = { diff.x * (j + 1),	tmpStartingPt.y };
				vec2Arr[3] = { diff.x * j,			tmpStartingPt.y };

				// --- SpriteList ---
				Sprite tmpSprite(vec2Arr);
				spriteListPtr->sprites.push_back(tmpSprite);
				// ------------------

				//std::cout << spriteListPtr->name << " frame added " << std::endl;

				--numOfFrames;

				if (numOfFrames <= 0)
					break;
			}

			tmpStartingPt.y += (diff.y);
		}

		GRAPHICS->AddSpriteList(spriteListPtr);
	}
}

inline void Texture::SerializeSameGrid(JsonReader& _jr, const std::string& _key1)
{
	// ---------------------------
	//	SubNum
	//	RowNum
	//	ColNum
	//	-------------
	//	SubName
	//	SubFrames
	// ---------------------------

	std::string textureName;

	_jr.ReadStringMember(_key1, "TextureName", textureName);

	std::string subName;
	int numOfFrames;

	int numOfSubs = 1;
	int rowNum, colNum;

	int maxFrameSum = 0;

	float frameDelay = 0.0f;
	float subFrameDelay = 0.0f;

	_jr.ReadIntMember(_key1, "SubNum", numOfSubs);
	_jr.ReadIntMember(_key1, "RowNum", rowNum);
	_jr.ReadIntMember(_key1, "ColNum", colNum);


	VEC2 diff = {	1.0f / colNum,
					1.0f / rowNum };


	VEC2 tmpStartingPt{ 0, 0 };
	std::array<VEC2, 4> vec2Arr;

	
	int j = 0;
	int k = 0;

	_jr.ReadFloatMember(_key1, "FrameDelay", frameDelay);


	// num of sub textures
	for (int i = 1; i <= numOfSubs; ++i)
	{
		// reset
		subFrameDelay = 0.0f;

		_jr.ReadStringMember(	_key1, "SubName" + std::to_string(i),		subName);
		_jr.ReadIntMember(		_key1, "SubFrames" + std::to_string(i),		numOfFrames);
		_jr.ReadFloatMember(	_key1, "SubFrameDelay" + std::to_string(i), subFrameDelay);
		
		maxFrameSum += numOfFrames;

		// --- SpriteList ---
		std::shared_ptr<SpriteList> spriteListPtr = std::make_shared<SpriteList>();

		spriteListPtr->name = subName;
		spriteListPtr->textureName = textureName;
		spriteListPtr->size = VEC2(	static_cast<float>(m_width) / colNum,
									static_cast<float>(m_height) / rowNum);
		// ------------------

		// --- Animated ---
		std::shared_ptr<Animation> animationPtr = std::make_shared<Animation>();

		animationPtr->name = subName;
		animationPtr->textureName = textureName;
		animationPtr->lastFrame = numOfFrames - 1;
		animationPtr->size = VEC2(	static_cast<float>(m_width) / colNum, 
									static_cast<float>(m_height) / rowNum);
		// ---------------

		// --- rows ---
		for (; j < rowNum; ++j)
		{
			// --- cols ---
			for (; k < colNum; ++k)
			{
				vec2Arr[0] = { diff.x * k		,	diff.y * (j + 1) };
				vec2Arr[1] = { diff.x * (k + 1) ,	diff.y * (j + 1) };
				vec2Arr[2] = { diff.x * (k + 1) ,	diff.y * j		 };
				vec2Arr[3] = { diff.x * k		,	diff.y * j		 };

				// --- SpriteList ---
				Sprite tmpSprite(vec2Arr);
				spriteListPtr->sprites.push_back(tmpSprite);
				// ------------------

				// --- Animated ---
				Frame tmpFrame(vec2Arr);

				// check for valid time delay and set it
				if (subFrameDelay != 0.0f)
					tmpFrame.timeDelay = subFrameDelay;
				else if (frameDelay != 0.0f)
					tmpFrame.timeDelay = frameDelay;

				animationPtr->frames.push_back(tmpFrame);
				// -----------------

				//std::cout << spriteListPtr->name << " frame added " << std::endl;

				if ((j * colNum + k) >= maxFrameSum)
					break;
			}

			if ((j * colNum + k) >= maxFrameSum)
				break;

			// reset
			k = 0;
		}
		//std::cout << spriteListPtr->name << " added into lists" << std::endl;

		GRAPHICS->AddSpriteList(spriteListPtr);
		GRAPHICS->AddAnimaton(animationPtr);
	}

}



// --- public ---

Texture::Texture()
	: m_rendererID{ 0 }, m_width{ 0 }, m_height{ 0 }, textureID{ 0 }
{
}

Texture::Texture(int _width, int _height)
	: m_rendererID{ 0 }, m_width{ _width }, m_height{ _height }, textureID{ 0 }
{
	glGenTextures(1, &m_rendererID);
	glBindTexture(GL_TEXTURE_2D, m_rendererID);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, m_width, m_height, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_rendererID, 0);
}

Texture::Texture(const std::string& path)
	: m_rendererID{ 0 }, m_width{ 0 }, m_height{ 0 }, textureID{ 0 }
{
	std::string m_filePath;
	unsigned char* m_localBuffer;

	/* -- to load in texture  -- */
	// remember to flip the texture vertically, open gl (0,0) is bottom left
	m_localBuffer = SOIL_load_image(path.c_str(), &m_width, &m_height, 0, SOIL_LOAD_RGBA);

	// generate and bind texture
	glGenTextures(1, &m_rendererID);
	glBindTexture(GL_TEXTURE_2D, m_rendererID);

	// for scaling
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);		//GL_NEAREST
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	GLCall(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, m_width, m_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, m_localBuffer));


	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_rendererID, 0);

	if (m_localBuffer)
		SOIL_free_image_data(m_localBuffer); // free the m_localBuffer.
}

Texture::~Texture()
{
	glDeleteTextures(1, &m_rendererID);
}

void Texture::Bind(unsigned int slot) const
{
	glActiveTexture(GL_TEXTURE0 + slot);			// KIV: slot not used yet
	glBindTexture(GL_TEXTURE_2D, m_rendererID);
}

void Texture::Unbind() const
{
	glBindTexture(GL_TEXTURE_2D, 0);
}

void Texture::Serialize(JsonReader& _jr, const std::string& _key1)
{
	std::string type; 

	_jr.ReadStringMember(_key1, "Type", type);

	if (type == "SameGrid")
		SerializeSameGrid(_jr, _key1);
	else if (type == "DiffGrid")
		SerializeDiffGrid(_jr, _key1);
	else
		assert(0 && "Texture type not recognised!");

	_jr.ReadStringMember(_key1, "TextureName", name);
}

void Texture::SetTextureID(int _textureID)
{
	textureID = _textureID;
}


int Texture::GetMaxFrame(std::string _name) const
{
	auto it = subTextureLst.find(_name);
	if (it == std::end(subTextureLst))
	{
		// texture not found
		assert(0);
	}

	return it->second->first.maxFrame;
}

//unsigned int Texture::GetNumCol(std::string _name) const
//{
//	auto it = subTextureLst.find(_name);
//	if (it == std::end(subTextureLst))
//	{
//		// texture not found
//		assert(0);
//	}
//
//	return it->second->first.numCol;
//}
//
//unsigned int Texture::GetNumRow(std::string _name) const
//{
//	auto it = subTextureLst.find(_name);
//	if (it == std::end(subTextureLst))
//	{
//		// texture not found
//		assert(0);
//	}
//
//	return it->second->first.numRow;
//}

VEC2 Texture::GetSize(std::string _name) const
{
	auto it = subTextureLst.find(_name);

	// safety checks
	assert(it != std::end(subTextureLst) && "Texture not found!");

	return it->second->first.size; // it->second->first.size;
}

std::string Texture::GetName() const
{
	return name;
}

std::array<VEC2, 4> Texture::GetUVs(std::string _name, unsigned int _frameNum) const
{
	auto it = subTextureLst.find(_name);

	// safety checks
	assert(it != std::end(subTextureLst) && "Texture not found!");

	return it->second->second.at(_frameNum);
}
