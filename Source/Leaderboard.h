#pragma once
#include "raylib_wrapper.h"
#include <string>
#include <vector>

struct PlayerData
{
	std::string name;
	int score;
};

class Leaderboard
{
public:
	Leaderboard();

	bool CheckNewHighScore(int score) const;
	void InsertNewHighScore(const std::string& name, int score);
	void SaveLeaderboard();

	// High score name entry UI
	void UpdateHighScoreNameEntry();
	void RenderHighscoreEntry();
	void RenderHighscoreNameInput();
	void RenderLeaderboard();

	bool IsEnteringName() const { return enteringName; }
	void StartNameEntry() { enteringName = true; }
	void ResetNameEntry();
	
	std::string GetEnteredName() const { return std::string(name); }

private:
	std::vector<PlayerData> entries;
	
	// Name input state
	char name[9 + 1] = "\0";
	int letterCount = 0;
	Rectangle textBox = { 600, 500, 225, 50 };
	bool mouseOnText = false;
	int framesCounter = 0;
	bool enteringName = false;

	// Helper methods
	void UpdateMouseCursor();
	void HandleTextInput();
	void HandleBackspace();
	void UpdateFrameCounter();
	bool IsNameValid() const;
	void HandleNameSubmission();
};
