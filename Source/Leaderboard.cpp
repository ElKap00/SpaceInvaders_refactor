#include "leaderboard.h"
#include <iostream>
#include <fstream>

Leaderboard::Leaderboard()
{
	// Initialize with default leaderboard entries
	entries_ = { 
		{"Player 1", 500}, 
		{"Player 2", 400}, 
		{"Player 3", 300}, 
		{"Player 4", 200}, 
		{"Player 5", 100} 
	};
}

bool Leaderboard::isNewHighScore(int score) const
{
	if (score > entries_[4].score_)
	{
		return true;
	}

	return false;
}

void Leaderboard::insertNewHighScore(const std::string& name, int score)
{
	PlayerData newData;
	newData.name_ = name;
	newData.score_ = score;

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
		// NOTE: Only allow keys in range [32..125]
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
		// Don't insert here - will be done externally with actual score
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
	// If no highscore or name is entered, show scoreboard and call it a day
	DrawText("PRESS ENTER TO CONTINUE", 600, 200, 40, YELLOW);

	DrawText("LEADERBOARD", 50, 100, 40, YELLOW);

	for (int i = 0; i < entries_.size(); i++)
	{
		char* tempNameDisplay = entries_[i].name_.data();
		DrawText(tempNameDisplay, 50, 140 + (i * 40), 40, YELLOW);
		DrawText(TextFormat("%i", entries_[i].score_), 350, 140 + (i * 40), 40, YELLOW);
	}
}

void Leaderboard::renderHighScoreNameInput()
{
	if (isTextBoxHovered_)
	{
		if (letterCount_ < 9)
		{
			// Draw blinking underscore char
			if (((cursorFrameCounter_ / 20) % 2) == 0)
			{
				DrawText("_", (int)textBox_.x + 8 + MeasureText(name_, 40), (int)textBox_.y + 12, 40, MAROON);
			}
		}
		else
		{
			//Name needs to be shorter
			DrawText("Press BACKSPACE to delete chars...", 600, 650, 20, YELLOW);
		}
	}

	// Explain how to continue when name is input
	if (letterCount_ > 0 && letterCount_ < 9)
	{
		DrawText("PRESS ENTER TO CONTINUE", 600, 800, 40, YELLOW);
	}
}

void Leaderboard::renderHighScoreEntry()
{
	DrawText("NEW HIGHSCORE!", 600, 300, 60, YELLOW);

	// BELOW CODE IS FOR NAME INPUT RENDER
	DrawText("PLACE MOUSE OVER INPUT BOX!", 600, 400, 20, YELLOW);

	DrawRectangleRec(textBox_, LIGHTGRAY);
	if (isTextBoxHovered_)
	{
		// HOVER CONFIRMIATION
		DrawRectangleLines((int)textBox_.x, (int)textBox_.y, (int)textBox_.width, (int)textBox_.height, RED);
	}
	else
	{
		DrawRectangleLines((int)textBox_.x, (int)textBox_.y, (int)textBox_.width, (int)textBox_.height, DARKGRAY);
	}

	//Draw the name being typed out
	DrawText(name_, (int)textBox_.x + 5, (int)textBox_.y + 8, 40, MAROON);

	//Draw the text explaining how many characters are used
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
