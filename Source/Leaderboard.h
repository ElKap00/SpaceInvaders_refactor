#pragma once
#include "raylib_wrapper.h"
#include <string>
#include <vector>
#include <iostream>
#include <fstream>
#include <algorithm>

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

public:
	bool isNewHighScore(int score) const
	{
		return std::any_of(entries_.begin(), entries_.end(),
			[score](const PlayerData& entry) {
				return score > entry.score_;
			});
	}

	void insertNewHighScore(const int score)
	{
		PlayerData newData{ name_, score };
		entries_.push_back(newData);

		std::sort(entries_.begin(), entries_.end(),
			[](const PlayerData& a, const PlayerData& b) {
				return a.score_ > b.score_;
			});

		if (entries_.size() > 5)
		{
			entries_.pop_back();
		}
	}

	void updateMouseCursor()
	{
		if (CheckCollisionPointRec(GetMousePosition(), textBox_))
		{
			isTextBoxHovered_ = true;
			SetMouseCursor(MOUSE_CURSOR_IBEAM);
		}
		else
		{
			isTextBoxHovered_ = false;
			SetMouseCursor(MOUSE_CURSOR_DEFAULT);
		}
	}

	void handleTextInput()
	{
		const int key = GetCharPressed();

		if ((key >= 32) && (key <= 125) && (name_.size() < 9))
		{
			name_ += static_cast<char>(key);
		}
	}

	void handleBackspace() noexcept
	{
		if (IsKeyPressed(KEY_BACKSPACE) && !name_.empty())
		{
			name_.pop_back();
		}
	}

	void updateFrameCounter() noexcept
	{
		if (isTextBoxHovered_)
		{
			cursorFrameCounter_++;
		}
		else
		{
			cursorFrameCounter_ = 0;
		}
	}

	void updateHighScoreNameEntry()
	{
		updateMouseCursor();

		if (isTextBoxHovered_)
		{
			handleTextInput();
			handleBackspace();
		}

		updateFrameCounter();

		if (IsKeyReleased(KEY_ENTER))
		{
			isEnteringName_ = false;
		}
	}

	void renderLeaderboard()
	{
		DrawText("PRESS ENTER TO CONTINUE", 600, 200, 40, YELLOW);

		DrawText("LEADERBOARD", 50, 100, 40, YELLOW);

		for (int i = 0; i < std::ssize(entries_); i++)
		{
			const auto& entry = entries_[i];
			DrawText(entry.name_.data(), 50, 140 + (i * 40), 40, YELLOW);
			DrawText(TextFormat("%i", entry.score_), 350, 140 + (i * 40), 40, YELLOW);
		}
	}

	void renderHighScoreNameInput()
	{
		if (name_.size() >= 9)
		{
			DrawText("Press BACKSPACE to delete chars...", 600, 650, 20, YELLOW);
			return;
		}

		if (isTextBoxHovered_ && (cursorFrameCounter_ / 20) % 2 == 0)
		{
			DrawText("_", static_cast<int>(textBox_.x) + 8 + MeasureText(name_.c_str(), 40), static_cast<int>(textBox_.y) + 12, 40, MAROON);
		}

		if (!name_.empty() && name_.size() < 9)
		{
			DrawText("PRESS ENTER TO CONTINUE", 600, 800, 40, YELLOW);
		}
	}

	void renderHighScoreEntry()
	{
		DrawText("NEW HIGHSCORE!", 600, 300, 60, YELLOW);

		DrawText("PLACE MOUSE OVER INPUT BOX!", 600, 400, 20, YELLOW);

		renderTextBox();

		DrawText(TextFormat("INPUT CHARS: %i/%i", static_cast<int>(name_.size()), 8), 600, 600, 20, YELLOW);
	}

	void renderTextBox()
	{
		DrawRectangleRec(textBox_, LIGHTGRAY);
		if (isTextBoxHovered_)
		{
			DrawRectangleLines(textBox_, RED);
		}
		else
		{
			DrawRectangleLines(textBox_, DARKGRAY);
		}
		DrawText(name_.c_str(), static_cast<int>(textBox_.x) + 5, static_cast<int>(textBox_.y) + 8, 40, MAROON);
	}

	bool getIsEnteringName() const noexcept { return isEnteringName_; }

	void startNameEntry() noexcept { isEnteringName_ = true; }

	void resetNameEntry()
	{
		name_.clear();
		isTextBoxHovered_ = false;
		cursorFrameCounter_ = 0;
		isEnteringName_ = false;
	}

};