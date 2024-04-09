#pragma once
#include <vector>
#include "Node.h"
#include "Physics.h"
#include "Logic.h"
// loop through environment
//stdarray throw in environemnt
//#define uint unsigned int

//#define iHEIGHT 3899
//#define iWIDTH 4559
#define cHEIGHT 64
#define cWIDTH 64
//#define gCOLUMN (MapWidth / cWIDTH)
//#define gROW (MapHeight / cHEIGHT)
//#define iHEIGHT (cHEIGHT * MapHeight) 
//#define iWIDTH (cWIDTH * MapWidth)

/*/
VEC2 GridtoWorld(VEC2 gridPos);
VEC2 WorldtoGrid(VEC2 worldPos);
/
*/

Node* AllocateMem();

// Return status of position / grid index
bool TestGridStat(VEC2 a);
bool TestGridStat(int x, int y);

void SetAllTrue(std::vector <std::vector<bool>>& all);
void SetAllFalse(std::vector <std::vector<bool>>& all);

// Set status using position
void SetTrue(std::vector <std::vector<bool>>& all, VEC2 pos, int xCollide, int yCollide);
void SetFalse(std::vector <std::vector<bool>>& all, VEC2 pos, int xCollide, int yCollide);

void SetTrue(std::vector <std::vector<bool>>& all, int x, int y);
void SetFalse(std::vector <std::vector<bool>>& all, int x, int y);

// Set specific status at specific grid index
void SetState(std::vector <std::vector<bool>>& all, int x, int y, bool s);

// Get Grid index using position
int GetGridX_index(VEC2 v);
int GetGridY_index(VEC2 v);

VEC2 GetGrid_index(VEC2 v);

// Get Position using Grid Index
VEC2 GetPos_index(int x, int y);

// Return a vector of waypoints, given src & destination
std::vector<VEC2> FindPath(std::vector <std::vector<bool>>& all, VEC2 src, VEC2 des);

// Check direction, no checking of blockage
bool CheckAllSide(VEC2 Npos, int range);

// Check one direction
bool CheckOneSide(VEC2 Npos, int range, int v);

// Check blockage
// false = cannot direct, true = cannot direct
bool CheckRoadCutoff(int gridx, int gridy);

// Check if player is in range of npc
bool DetectedPlayer(const VEC2& Npos, int dir, float range);
