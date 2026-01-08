#include "leaderboard.h"
#include <iostream>
#include <fstream>

Leaderboard::Leaderboard()
{
	entries_ = { 
		{"Player 1", 500}, 
		{"Player 2", 400}, 
		{"Player 3", 300}, 
		{"Player 4", 200}, 
		{"Player 5", 100} 
	};
}

bool Leaderboard::isNewHighScore(int score) const //TODO: use an algorithm 
{
	return (score > entries_[4].score_);
}

// TODO: change to string_view to avoid copy
void Leaderboard::insertNewHighScore(const std::string& name, int score)
{
	PlayerData newData{ name, score };
	//TODO: use algorithms. 
	// for example: push_back the new score. 
	// sort the scores. 
	// pop_back if the list is too long.

	for (int i = 0; i < entries_.size(); i++)
	{
		if (newData.score_ > entries_[i].score_)
		{
			entries_.insert(entries_.begin() + i, newData);
			entries_.pop_back();
			i = entries_.size();
		}
	}
}

void Leaderboard::updateMouseCursor()
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

void Leaderboard::handleTextInput()
{
	int key = GetCharPressed();

	while (key > 0)
	{
		// TODO: fix C-style casts and indexing
		if ((key >= 32) && (key <= 125) && (letterCount_ < 9))
		{
			name_[letterCount_] = (char)key;
			name_[letterCount_ + 1] = '\0';
			letterCount_++;
		}

		key = GetCharPressed();
	}
}

void Leaderboard::handleBackspace()
{
	//TODO: std::string::pop_back()
	if (IsKeyPressed(KEY_BACKSPACE))
	{
		letterCount_--;
		if (letterCount_ < 0)
		{
			letterCount_ = 0;
		}
		name_[letterCount_] = '\0';
	}
}

void Leaderboard::updateFrameCounter()
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

bool Leaderboard::isNameValid() const
{
	return letterCount_ > 0 && letterCount_ < 9;
}

void Leaderboard::handleNameSubmission()
{
	if (isNameValid() && IsKeyReleased(KEY_ENTER))
	{
		isEnteringName_ = false;
	}
}

void Leaderboard::updateHighScoreNameEntry()
{
	updateMouseCursor();

	if (isTextBoxHovered_)
	{
		handleTextInput();
		handleBackspace();
	}

	updateFrameCounter();
	handleNameSubmission();
}

void Leaderboard::renderLeaderboard()
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

void Leaderboard::renderHighScoreNameInput()
{
	if (isTextBoxHovered_) //reverse and quick bail. Move code left
	{
		if (letterCount_ < 9)
		{
			// TODO: fix C-style casts
			if (((cursorFrameCounter_ / 20) % 2) == 0)
			{
				DrawText("_", (int)textBox_.x + 8 + MeasureText(name_, 40), (int)textBox_.y + 12, 40, MAROON);
			}
		}
		else
		{
			DrawText("Press BACKSPACE to delete chars...", 600, 650, 20, YELLOW);
		}
	}

	if (letterCount_ > 0 && letterCount_ < 9)
	{
		DrawText("PRESS ENTER TO CONTINUE", 600, 800, 40, YELLOW);
	}
}

void Leaderboard::renderHighScoreEntry()
{
	DrawText("NEW HIGHSCORE!", 600, 300, 60, YELLOW);

	DrawText("PLACE MOUSE OVER INPUT BOX!", 600, 400, 20, YELLOW);

	DrawRectangleRec(textBox_, LIGHTGRAY);
	// TODO: fix C-style casts
	if (isTextBoxHovered_)
	{
		DrawRectangleLines((int)textBox_.x, (int)textBox_.y, (int)textBox_.width, (int)textBox_.height, RED);
	}
	else
	{
		DrawRectangleLines((int)textBox_.x, (int)textBox_.y, (int)textBox_.width, (int)textBox_.height, DARKGRAY);
	}

	DrawText(name_, (int)textBox_.x + 5, (int)textBox_.y + 8, 40, MAROON);

	DrawText(TextFormat("INPUT CHARS: %i/%i", letterCount_, 8), 600, 600, 20, YELLOW);
}

void Leaderboard::resetNameEntry()
{
	letterCount_ = 0;
	name_[0] = '\0';
	isTextBoxHovered_ = false;
	cursorFrameCounter_ = 0;
	isEnteringName_ = false;
}
