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
	Leaderboard() noexcept = default;
	~Leaderboard() noexcept = default;

	bool isNewHighScore(int score) const;
	void insertNewHighScore(int score);

	// High score name entry UI
	void updateHighScoreNameEntry();
	void renderHighScoreEntry();
	void renderHighScoreNameInput();
	void renderLeaderboard();

	bool getIsEnteringName() const noexcept { return isEnteringName_; }
	void startNameEntry() noexcept{ isEnteringName_ = true; }
	void resetNameEntry();

private:
	std::vector<PlayerData> entries_ = {
		{"Player 1", 500},
		{"Player 2", 400},
		{"Player 3", 300},
		{"Player 4", 200},
		{"Player 5", 100}
	};;
	
	// Name input state
	std::string name_;
	Rectangle textBox_ = { 600, 500, 225, 50 };
	int cursorFrameCounter_ = 0;
	bool isTextBoxHovered_ = false;
	bool isEnteringName_ = false;

	// Helper methods
	void updateMouseCursor();
	void handleTextInput();
	void handleBackspace();
	void renderTextBox();
	void updateFrameCounter();
};
