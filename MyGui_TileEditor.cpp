/*************************************************************************************/
/*
File Name:		MyGui_TileEditor.cpp
Project Name:	WhereGotLate
Author(s):		Kok Zhe Yan			 [100%]


Copyright Information:
All content © 2019 DigiPen (SINGAPORE) Corporation, all rights reserved.
*/
/*************************************************************************************/
#include "stdafx.h"

#include <filesystem>

#include <fstream>

#include "MyGui_TileEditor.h"
#include "MyGuiManager.h"

#include "Graphics.h"

namespace fs = std::filesystem;

static bool walltile = false;
static bool floortile = true;
static bool emptytile = false;

static int Size[2] = { 0,0 };

MyGui_TileEditor::MyGui_TileEditor()// : mSpriteID{ 0 }, mFrameNum{ 0 }
{
	rowvector newvector{ { "", 0} };
	tilemap.emplace_back(newvector);
}

MyGui_TileEditor::~MyGui_TileEditor()
{
	for (auto& row : tilemap)
	{
		row.clear();
	}
	tilemap.clear();

	SelectedSpriteList.clear();
}

void MyGui_TileEditor::Open()
{
}

void MyGui_TileEditor::Update()
{
	if (MyGuiManager::show_tile_editor)	LoadTileEditor(&MyGuiManager::show_tile_editor);
}

void MyGui_TileEditor::Exit()
{
}

void MyGui_TileEditor::LoadTileEditor(bool* _open)
{
	if (!ImGui::Begin("Tile Editor", _open))
	{
		ImGui::End();
		return;
	}

	ImVec2 buttonsize{ 50.0f, 50.0f };
	ImVec2 defaultuv3{ 0.0f, 0.0f };
	ImVec2 defaultuv1{ 1.0f, 1.0f };
	int spacing = 1;
	int imagebuttonid = 0;
	ImGuiWindowFlags window_flags = ImGuiWindowFlags_HorizontalScrollbar | ImGuiWindowFlags_AlwaysAutoResize;

	ImGui::Columns(2, "TileMap", true);

	if (ImGui::Button("Select Tile Map"))
	{
		ImGui::OpenPopup("TileMap");
	}
	ImGui::SameLine();
	ImGui::Checkbox("Empty Tile", &emptytile);

	if (ImGui::BeginPopupModal("TileMap"))
	{
		ImGui::BeginChild("Spritelist", ImVec2(200, 200), true);

		auto spritelist = GRAPHICS->GetSpriteList();

		static std::string currsprite = "";
		for (auto& sprite : spritelist)
		{
			bool isSelected = (currsprite == sprite->name);
			if (ImGui::Selectable(sprite->name.c_str(), isSelected))
			{
				currsprite = sprite->name;
			}
		}
		ImGui::EndChild();

		if (ImGui::Button("Confirm"))
		{
			mSpriteList = GRAPHICS->GetSpriteList(currsprite);
			SelectedSpriteList = currsprite;
			ImGui::CloseCurrentPopup();
		}

		if (ImGui::Button("Cancel"))
		{
			ImGui::CloseCurrentPopup();
		}

		ImGui::EndPopup();
	}

	// Tiles
	if (strcmp(SelectedSpriteList.c_str(), "") == 0)
	{
		ImGui::Text("No TileMap selected.");
	}
	else
	{
		ImGui::BeginChild("Child2", ImVec2(0, 0), false, window_flags);

		std::shared_ptr<SpriteList> spritelistptr = GRAPHICS->GetSpriteList(SelectedSpriteList);

		auto& spritelist = spritelistptr->sprites;

		int i = 0;
			
		for (auto& sprite : spritelist)
		{
			ImGui::PushID(imagebuttonid++);
			if (ImGui::ImageButton(reinterpret_cast<void*>(static_cast<intptr_t>(GRAPHICS->GetRenderID(spritelistptr->textureName))),
				buttonsize,
				ImVec2{ sprite.uv[3].x, sprite.uv[3].y },
				ImVec2{ sprite.uv[1].x, sprite.uv[1].y },
				spacing))
			{
				SelectedFrameNum = i;
			}
			ImGui::PopID();
			ImGui::SameLine(0, 0);

			if (++i % 6 == 0)
				ImGui::NewLine();
		}
		ImGui::EndChild();
	}

	// Tile map
	ImGui::NextColumn();

	ImGui::SliderInt2("Map Size", Size, 0, 1000);


	/*if (ImGui::Button("Add Column"))
	{
		for (auto& iter : tilemap)
		{
			iter.emplace_back("", 0);
		}

	}
	ImGui::SameLine();
	if (ImGui::Button("Add Row"))
	{
		rowvector newvector;
		for (int i = 0; i < tilemap[0].size(); ++i)
			newvector.emplace_back("", 0);
		tilemap.emplace_back(newvector);
	}*/
	rowvector newvector{ { "", 0} };
	tilemap.resize(Size[1], newvector);
	for (auto& rows : tilemap)
	{
		rows.resize(Size[0], { "", 0 });
	}

	//ImGui::SameLine();
	if (ImGui::Button("Load TileMap"))
	{
		ImGui::OpenPopup("Load");
	}
	ImGui::SameLine();
	if (ImGui::Button("Save TileMap"))
	{
		ImGui::OpenPopup("Save");
	}
	ImGui::SameLine();


	if (ImGui::Checkbox("Wall", &walltile))
	{
		floortile = !floortile;
	}
	ImGui::SameLine();
	if (ImGui::Checkbox("Floor", &floortile))
	{
		walltile = !walltile;
	}

	/*ImGui::SameLine();
	ImGui::Text();*/

	if (ImGui::BeginPopupModal("Load"))
	{
		ImGui::BeginChild("Tilemap Select", ImVec2(200, 200), true);

		static std::string mapselected;

		for (auto& p : fs::directory_iterator(CORE->mPath + "Tilemaps"))
		{
			bool isSelected = (mapselected == p.path().stem().string().c_str());
			if (ImGui::Selectable(p.path().stem().string().c_str(), isSelected))
			{
				mapselected = p.path().stem().string().c_str();
			}
		}

		ImGui::EndChild();

		if (ImGui::Button("Confirm"))
		{
			LoadTileMap(mapselected);
			ImGui::CloseCurrentPopup();
		}

		if (ImGui::Button("Cancel"))
		{
			ImGui::CloseCurrentPopup();
		}


		ImGui::EndPopup();
	}

	if (ImGui::BeginPopupModal("Save"))
	{
		ImGui::BeginChild("Scene Select", ImVec2(200, 200), true);

		static std::string sceneselected;

		for (auto& p : fs::directory_iterator(CORE->mPath + "Level Files"))
		{
			bool isSelected = (sceneselected == p.path().stem().string().c_str());
			if (ImGui::Selectable(p.path().stem().string().c_str(), isSelected))
			{
				sceneselected = p.path().stem().string().c_str();
			}
		}

		ImGui::EndChild();

		if (ImGui::Button("Confirm"))
		{
			SaveTileMap(sceneselected);
			ImGui::CloseCurrentPopup();
		}

		if (ImGui::Button("Cancel"))
		{
			ImGui::CloseCurrentPopup();
		}


		ImGui::EndPopup();
	}

	ImGui::BeginChild("Child1", ImVec2(0,0), false, window_flags);

	ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2{ 0, 0 });
	// Tiling of map
	for (auto &row : tilemap)
	{
		for (auto &column : row)
		{
			auto& [name, framenum] = column;

			unsigned int renderid = 0;
			ImVec2 uv3{};
			ImVec2 uv1{};

			if (strcmp(name.c_str(), ""))
			{
				std::shared_ptr<SpriteList> spritelist = GRAPHICS->GetSpriteList(name);

				Sprite& sprite = spritelist->sprites.at(framenum);

				renderid = GRAPHICS->GetRenderID(spritelist->textureName);

				uv3 = ImVec2{ sprite.uv[3].x, sprite.uv[3].y };
				uv1 = ImVec2{ sprite.uv[1].x, sprite.uv[1].y };
			}
			else
			{
				renderid = 0;

				uv3 = defaultuv3;
				uv1 = defaultuv1;
			}

			ImGui::PushID(imagebuttonid++);
			if (ImGui::ImageButton(reinterpret_cast<void*>(static_cast<intptr_t>(renderid)),
					buttonsize,
					uv3,
					uv1,
					spacing))
			{
				if (emptytile)
				{
					name = "";
					framenum = 0;
				}
				else
				{
					name = SelectedSpriteList;
					framenum = SelectedFrameNum;
				}
			}
			ImGui::SameLine(0,0);
			ImGui::PopID();
		}
		ImGui::NewLine();
		
	}
	ImGui::PopStyleVar(1);

	ImGui::EndChild();

	ImGui::End();

}

void MyGui_TileEditor::LoadTileMap(const std::string& _filename)
{
	JsonReader jr(CORE->mPath + "Tilemaps/" + _filename + ".json");
	bool isFileOpened = jr.IsFileOpened();
	assert(isFileOpened);

	jr.ParseStreamToDoc();
	jr.doc.Accept(jr.prettyReaderBuffer);

	if (!isFileOpened)
	{
		std::cout << "Error Loading level!" << std::endl;
		return;
	}
	else
	{
		//needed to reset the ifstream good() flag, otherwise for some reason the below jr.IsGood() will be false
		jr.stream.clear();
	}

	std::string row = "Row";
	std::string col = "Col";
	std::string rowkey, colkey;

	int width = 0;
	int height = 0;

	if (jr.doc.HasMember("Size"))
	{
		jr.ReadIntMember("Size", "Width", width);
		jr.ReadIntMember("Size", "Height", height);
	}

	Size[0] = width;
	Size[1] = height;

	
	for (auto& rows : tilemap)
	{
		rows.clear();
	}
	tilemap.clear();

	rowvector newvector{ { "", 0} };
	tilemap.resize(height, newvector);
	for (auto& rows : tilemap)
	{
		rows.resize(width, {"", 0});
	}

	for (int i = 0; i < height; ++i)
	{
		rowkey = row + std::to_string(i);
		if (jr.doc.HasMember(rowkey.c_str()))
		{
			for (int j = 0; j < width; ++j)
			{
				colkey = col + std::to_string(j);
				auto& [name, framenum] = tilemap[height - i - 1][j];

				jr.ReadStringMember(rowkey, colkey, "Texture name", name);
				jr.ReadIntMember(rowkey, colkey, "CurrFrame", framenum);
			}
		}
	}

	if (_filename.find("Wall") != std::string::npos)
	{
		walltile = true;
		floortile = false;
	}
	if (_filename.find("Floor") != std::string::npos)
	{
		walltile = false;
		floortile = true;
	}
}

void MyGui_TileEditor::SaveTileMap(const std::string& _scenename)
{
	JsonWriter jw;
	
	std::ofstream file;

	if (walltile)
	{
		file.open(CORE->mPath + "AI_Grid_Map/" + _scenename + ".json", std::ios::out | std::ios::trunc);

		assert(file.is_open());
	}

	jw.prettyWriterBuffer.StartObject();

	jw.CreateKey("Size");
	jw.prettyWriterBuffer.StartObject();
	jw.CreateIntMember("Width", static_cast<int>(tilemap[0].size()));
	jw.CreateIntMember("Height", static_cast<int>(tilemap.size()));
	jw.prettyWriterBuffer.EndObject();

	for (int i = 0; i < tilemap.size(); ++i)
	{
		jw.CreateKey("Row" + std::to_string(i));
		jw.prettyWriterBuffer.StartObject();

		int j = 0;
		for (auto column : tilemap[tilemap.size() - i - 1])
		{
			auto [name, framenum] = column;
			
			jw.CreateKey("Col" + std::to_string(j++));
			jw.prettyWriterBuffer.StartObject();
			jw.CreateStringMember("Texture name", name);
			jw.CreateIntMember("CurrFrame", framenum);
			jw.prettyWriterBuffer.EndObject();

			if (!walltile)
				continue;

			if (strcmp("", name.c_str()) == 0)
			{
				file << 1;
			}
			else
			{
				file << 0;
			}
		}
		if (walltile)
			file << "\n";
		jw.prettyWriterBuffer.EndObject();
	}

	jw.prettyWriterBuffer.EndObject();

	jw.WriteToJsonFile(CORE->mPath + "Tilemaps/" + _scenename + (walltile ? "Wall" : "Floor") + ".json");
}

//void* MyGui_TileEditor::operator new(size_t size)
//{
//	std::cout << " allocate " << size << " bytes." << std::endl;
//	void* result = malloc(size);
//	if (result == nullptr)
//	{
//		throw std::bad_alloc();
//	}
//	//++num;
//	std::cout << (void*)result << std::endl;
//	return result;
//}
//
//void MyGui_TileEditor::operator delete(void* pointer)
//{
//	std::cout << " deallocate." << std::endl;
//	if (pointer != nullptr)
//	{
//		free(pointer);
//	}
//
//	std::ofstream myfile(CORE->mPath + "debug.txt");
//	if (myfile.is_open())
//	{
//		myfile << (void*)pointer;
//		myfile << "\n";
//		myfile.close();
//	}
//
//}
