#pragma once
#include "raylib_wrapper.h"
#include <string>
#include <vector>

struct PlayerData
{
	std::string name_;
	int score_;
};

class Leaderboard
{
public:
	Leaderboard();

	bool isNewHighScore(int score) const;
	void insertNewHighScore(const std::string& name, int score);
	void saveLeaderboard();

	// High score name entry UI
	void updateHighScoreNameEntry();
	void renderHighScoreEntry();
	void renderHighScoreNameInput();
	void renderLeaderboard();

	bool isEnteringName() const { return isEnteringName_; }
	void startNameEntry() { isEnteringName_ = true; }
	void resetNameEntry();
	
	std::string getEnteredName() const { return std::string(name_); }

private:
	std::vector<PlayerData> entries_;
	
	// Name input state
	char name_[9 + 1] = "\0";
	int letterCount_ = 0;
	Rectangle textBox_ = { 600, 500, 225, 50 };
	bool isTextBoxHovered_ = false;
	int cursorFrameCounter_ = 0;
	bool isEnteringName_ = false;

	// Helper methods
	void updateMouseCursor();
	void handleTextInput();
	void handleBackspace();
	void updateFrameCounter();
	bool isNameValid() const;
	void handleNameSubmission();
};
