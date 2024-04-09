///*************************************************************************************/
///*
//File Name:		GraphicsLayer.cpp
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
//
//// NOT USED ANYMORE
#include "stdafx.h"
#include "GraphicsLayer.h"
//
//
//// KIV to remove
//#include <Windows.h>		//UNREFERENCED_PARAMETER
//
//
//GraphicsLayer::GraphicsLayer(unsigned int _ID)
//	: name{ "Layer" }, isEnabled{ true }, spriteCounter{ 0 }, m_ID{ _ID }
//{
//
//}
//
//GraphicsLayer::~GraphicsLayer()
//{
//}
//
//void GraphicsLayer::Update()
//{
//}
//
//unsigned int GraphicsLayer::AddSprite(SpriteComponent* _spr)
//{
//
//	UNREFERENCED_PARAMETER(_spr);
//
//	spriteMap.insert({ spriteCounter , _spr });
//
//	//spriteList.push_back(_spr);
//
//	return spriteCounter++;
//}
//
//void GraphicsLayer::RemoveSprite(unsigned int _sprID)
//{
//	UNREFERENCED_PARAMETER(_sprID);
//
//	spriteMap.erase(_sprID);
//
//}
//
//void GraphicsLayer::Draw(	std::shared_ptr<Shader> _shaderPtr, 
//							std::shared_ptr<VertexArray> _vaPtr, 
//							std::shared_ptr<IndexBuffer> _ibPtr,
//							glm::mat4 _vp)
//{
//	//for (std::pair<const unsigned int, Sprite>& keyValue : spriteMap)
//	//{
//	//	keyValue.second.Draw();
//	//}
//
//	// KIV: weird warning happens here, not consisten tho.
//	
//	UNREFERENCED_PARAMETER(_vp);
//
//	_shaderPtr->Bind();
//	_vaPtr->Bind();
//	_ibPtr->Bind();
//
//	//std::for_each(
//	//	std::begin(spriteMap),
//	//	std::end(spriteMap),
//	//	[&](/*const std::pair<const unsigned int, Sprite>&*/ auto& myPair)
//	//	{
//	//		myPair.second.Draw(_shaderPtr, _vaPtr, _ibPtr, _vp);
//	//	}
//	//);
//
//	//std::map<unsigned int, Sprite*>::iterator it;
//	//for (it = std::begin(spriteMap); it != std::end(spriteMap); ++it)
//	//{
//	//	//it->second->Draw(_shaderPtr, _vaPtr, _ibPtr, _vp);
//	//}
//
//}
//
//void GraphicsLayer::DrawWithDebug(std::shared_ptr<Shader> _shaderPtr, std::shared_ptr<VertexArray> _vaPtr, std::shared_ptr<IndexBuffer> _ibPtr, glm::mat4 _vp)
//{
//	UNREFERENCED_PARAMETER(_vp);
//
//	_shaderPtr->Bind();
//	_vaPtr->Bind();
//	_ibPtr->Bind();
//
//	//std::for_each(
//	//	std::begin(spriteMap),
//	//	std::end(spriteMap),
//	//	[&](/*const std::pair<const unsigned int, Sprite>&*/ auto& myPair)
//	//	{
//	//		//myPair.second->DrawWithDebug(_shaderPtr, _vaPtr, _ibPtr, _vp);
//	//	}
//	//);
//}
//
//Sprite* GraphicsLayer::GetSprite(unsigned int _num)
//{
//	return spriteMap[_num];
//}
//
//void GraphicsLayer::ClearAll()
//{
//	spriteMap.clear();
//}
//
//std::map<unsigned int, Sprite*>& GraphicsLayer::GetSpriteMap()
//{
//	return spriteMap;
//
//}
//
