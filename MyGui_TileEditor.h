/*************************************************************************************/
/*
File Name:		MyGui_TileEditor.h
Project Name:	WhereGotLate
Author(s):		Kok Zhe Yan			 [100%]


Copyright Information:
All content © 2019 DigiPen (SINGAPORE) Corporation, all rights reserved.
*/
/*************************************************************************************/
#pragma once
#include "MyGui.h"

#include "SpriteList.h"

class MyGui_TileEditor : public MyGui
{
	void LoadTileEditor(bool* _open);

	void LoadTileMap(const std::string& _filename);

	void SaveTileMap(const std::string& _scenename);

	std::weak_ptr<SpriteList> mSpriteList;

	std::string SelectedSpriteList;

	int SelectedFrameNum;

	// vector storing the rows of vector holding the data of the tiles
	//using rowvector = std::vector<std::tuple<std::weak_ptr<Sprite>, int>>;
	//using rowvector = std::vector<std::weak_ptr<Sprite>>;
	using rowvector = std::vector<std::tuple<std::string, int>>;
	std::vector<rowvector> tilemap;

	//int num;



public:
	MyGui_TileEditor();

	~MyGui_TileEditor();

	void Open() override;

	void Update() override;

	void Exit() override;



};

