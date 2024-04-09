#include "stdafx.h"
#include "Score.h"
#include "graphics.h"
#include "CoreEngine.h"
#include <string>


ScoreManager::ScoreManager() :
	totalStagesCleared{0},
	totalNotesCollected{0},
	stageNotesCollected{0},
	stageCleared{0},
	rendererPtr{nullptr}
{
}

ScoreManager::~ScoreManager()
{
}

void ScoreManager::Initialize()
{
	rendererPtr = GRAPHICS->GetTextRenderer();
}

void ScoreManager::Update(float _dt)
{
	UNREFERENCED_PARAMETER(_dt);
}

void ScoreManager::Clear()
{
	totalStagesCleared = 0;
	totalNotesCollected = 0;
	stageNotesCollected = 0;
	stageCleared = 0;
}

void ScoreManager::clearStageVars()
{
	stageCleared = 0;
	stageNotesCollected = 0;
}

void ScoreManager::clearTotalStageVars()
{
	totalNotesCollected = 0;
	totalStagesCleared = 0;
}

void ScoreManager::clearAllStageVars()
{
	clearStageVars();
	clearTotalStageVars();
}

void ScoreManager::displayStageCleared()
{
	if (!CORE->GetPausedStatus())
	{
		std::string fullScore = std::to_string(stageCleared) + " / 1";
		std::string notes = std::to_string(stageNotesCollected) + " / 2 ";
		rendererPtr->AddText(fullScore, VEC2(450.0f, 430.0f));
		rendererPtr->AddText(notes, VEC2(450.0f, 325.0f));
	}

}

void ScoreManager::displayTotalStats()
{
	if (!CORE->GetPausedStatus())
	{
		std::string fullScore = std::to_string(totalStagesCleared) + " / 4";
		std::string notes = std::to_string(totalNotesCollected) + " / 8 ";
		rendererPtr->AddText(fullScore, VEC2(450.0f, 430.0f));
		rendererPtr->AddText(notes, VEC2(450.0f, 325.0f));
	}
	
}
