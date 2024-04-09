#pragma once
#include "stdafx.h"
#include "Singleton.h"
#include "TextRenderer.h"

class ScoreManager : public Singleton<ScoreManager>
{
	//these are for at the end
	int totalStagesCleared;//keeps the player's num of cleared stages
	int totalNotesCollected;//keeps total num of notes collected

	//these ones are more for indiv stage's win/lose screens
	int stageNotesCollected; // for displaying notes collected in stage
	int stageCleared; //for displaying stage cleared 

	friend class Singleton<ScoreManager>;

	std::shared_ptr<TextRenderer> rendererPtr;


public:
	ScoreManager();
	~ScoreManager();

	void Initialize();
	virtual void Update(float _dt);
	void Clear();

	void incrementStages() { totalStagesCleared++; stageCleared++;  };
	void incrementNotes() { totalNotesCollected++; stageNotesCollected++; };
	void clearStageVars();
	void clearTotalStageVars();
	void clearAllStageVars();

	int getStageCleared() { return stageCleared;  };
	int getStageNotesCollected() { return stageNotesCollected;  };
	int getTotalStageClears() { return totalStagesCleared;  };
	int getTotalNotesCollected() { return totalNotesCollected;  };

	void displayStageCleared();
	void displayTotalStats();
	//void displayNotes();
};

#define MyScore (ScoreManager::GetInstance())