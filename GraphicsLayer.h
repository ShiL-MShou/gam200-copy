///*************************************************************************************/
///*
//File Name:		GraphicsLayer.h
//Project Name:	WhereGotLate
//Author(s):		Tay Qin Wen Alvin [100%]
//
//
//Brief:		This is used to segregate the sprites to imitate the depth for sprites.
//			This was used when the z-buffer was not up, and is no longer in use.
//
//
//Copyright Information:
//All content © 2019 DigiPen (SINGAPORE) Corporation, all rights reserved.
//*/
///*************************************************************************************/
//
//// NOT USED ANYMORE
#pragma once
//
//#include <string>
//#include <map>
//#include <algorithm>
//#include "Sprite.h"
//
//class GraphicsLayer
//{
//	std::string name;
//	bool isEnabled;
//
//	std::map<unsigned int, Sprite*> spriteMap;
//
//	unsigned int m_ID;
//	//std::vector<Sprite> spriteList;
//
//	unsigned int spriteCounter;
//
//public:
//	GraphicsLayer(unsigned int _ID);
//	~GraphicsLayer();
//
//	void Update();
//	unsigned int AddSprite(Sprite* _spr);
//	void RemoveSprite(unsigned int _sprID);
//
//	void Draw(	std::shared_ptr<Shader> _shaderPtr, 
//				std::shared_ptr<VertexArray> _vaPtr, 
//				std::shared_ptr<IndexBuffer> _ibPtr,
//				glm::mat4 _vp);
//
//	void DrawWithDebug(	std::shared_ptr<Shader> _shaderPtr,
//						std::shared_ptr<VertexArray> _vaPtr,
//						std::shared_ptr<IndexBuffer> _ibPtr,
//						glm::mat4 _vp);
//
//	// setters and getters
//	Sprite* GetSprite(unsigned int _num);
//
//	void ClearAll();
//
//	std::map<unsigned int, Sprite*>& GetSpriteMap();
//
//	//void TestDraw(std::pair<unsigned int, Sprite>& pair)
//	//{
//	//	pair.second.Draw();
//	//}
//};
//
