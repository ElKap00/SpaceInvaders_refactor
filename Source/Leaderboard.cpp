#include "Leaderboard.h"
#include <iostream>
#include <fstream>

Leaderboard::Leaderboard()
{
	// Initialize with default leaderboard entries
	entries = { 
		{"Player 1", 500}, 
		{"Player 2", 400}, 
		{"Player 3", 300}, 
		{"Player 4", 200}, 
		{"Player 5", 100} 
	};
}

bool Leaderboard::CheckNewHighScore(int score) const
{
	if (score > entries[4].score)
	{
		return true;
	}

	return false;
}

void Leaderboard::InsertNewHighScore(const std::string& name, int score)
{
	PlayerData newData;
	newData.name = name;
	newData.score = score;

	for (int i = 0; i < entries.size(); i++)
	{
		if (newData.score > entries[i].score)
		{
			entries.insert(entries.begin() + i, newData);
			entries.pop_back();
			i = entries.size();
		}
	}
}

// TODO: improve error handling
// TODO: close file properly?
void Leaderboard::SaveLeaderboard()
{
	// SAVE LEADERBOARD AS ARRAY

	// OPEN FILE
	std::fstream file;

	file.open("Leaderboard");

	if (!file)
	{
		std::cout << "file not found \n";
	}
	else
	{
		std::cout << "file found \n";
	}
	// CLEAR FILE

	// WRITE ARRAY DATA INTO FILE

	// CLOSE FILE
}

void Leaderboard::UpdateMouseCursor()
{
	if (CheckCollisionPointRec(GetMousePosition(), textBox))
	{
		mouseOnText = true;
		SetMouseCursor(MOUSE_CURSOR_IBEAM);
	}
	else
	{
		mouseOnText = false;
		SetMouseCursor(MOUSE_CURSOR_DEFAULT);
	}
}

void Leaderboard::HandleTextInput()
{
	int key = GetCharPressed();

	while (key > 0)
	{
		// NOTE: Only allow keys in range [32..125]
		if ((key >= 32) && (key <= 125) && (letterCount < 9))
		{
			name[letterCount] = (char)key;
			name[letterCount + 1] = '\0';
			letterCount++;
		}

		key = GetCharPressed();
	}
}

void Leaderboard::HandleBackspace()
{
	if (IsKeyPressed(KEY_BACKSPACE))
	{
		letterCount--;
		if (letterCount < 0)
		{
			letterCount = 0;
		}
		name[letterCount] = '\0';
	}
}

void Leaderboard::UpdateFrameCounter()
{
	if (mouseOnText)
	{
		framesCounter++;
	}
	else
	{
		framesCounter = 0;
	}
}

bool Leaderboard::IsNameValid() const
{
	return letterCount > 0 && letterCount < 9;
}

void Leaderboard::HandleNameSubmission()
{
	if (IsNameValid() && IsKeyReleased(KEY_ENTER))
	{
		// Don't insert here - will be done externally with actual score
		enteringName = false;
	}
}

void Leaderboard::UpdateHighScoreNameEntry()
{
	UpdateMouseCursor();

	if (mouseOnText)
	{
		HandleTextInput();
		HandleBackspace();
	}

	UpdateFrameCounter();
	HandleNameSubmission();
}

void Leaderboard::RenderLeaderboard()
{
	// If no highscore or name is entered, show scoreboard and call it a day
	DrawText("PRESS ENTER TO CONTINUE", 600, 200, 40, YELLOW);

	DrawText("LEADERBOARD", 50, 100, 40, YELLOW);

	for (int i = 0; i < entries.size(); i++)
	{
		char* tempNameDisplay = entries[i].name.data();
		DrawText(tempNameDisplay, 50, 140 + (i * 40), 40, YELLOW);
		DrawText(TextFormat("%i", entries[i].score), 350, 140 + (i * 40), 40, YELLOW);
	}
}

void Leaderboard::RenderHighscoreNameInput()
{
	if (mouseOnText)
	{
		if (letterCount < 9)
		{
			// Draw blinking underscore char
			if (((framesCounter / 20) % 2) == 0)
			{
				DrawText("_", (int)textBox.x + 8 + MeasureText(name, 40), (int)textBox.y + 12, 40, MAROON);
			}
		}
		else
		{
			//Name needs to be shorter
			DrawText("Press BACKSPACE to delete chars...", 600, 650, 20, YELLOW);
		}
	}

	// Explain how to continue when name is input
	if (letterCount > 0 && letterCount < 9)
	{
		DrawText("PRESS ENTER TO CONTINUE", 600, 800, 40, YELLOW);
	}
}

void Leaderboard::RenderHighscoreEntry()
{
	DrawText("NEW HIGHSCORE!", 600, 300, 60, YELLOW);

	// BELOW CODE IS FOR NAME INPUT RENDER
	DrawText("PLACE MOUSE OVER INPUT BOX!", 600, 400, 20, YELLOW);

	DrawRectangleRec(textBox, LIGHTGRAY);
	if (mouseOnText)
	{
		// HOVER CONFIRMIATION
		DrawRectangleLines((int)textBox.x, (int)textBox.y, (int)textBox.width, (int)textBox.height, RED);
	}
	else
	{
		DrawRectangleLines((int)textBox.x, (int)textBox.y, (int)textBox.width, (int)textBox.height, DARKGRAY);
	}

	//Draw the name being typed out
	DrawText(name, (int)textBox.x + 5, (int)textBox.y + 8, 40, MAROON);

	//Draw the text explaining how many characters are used
	DrawText(TextFormat("INPUT CHARS: %i/%i", letterCount, 8), 600, 600, 20, YELLOW);
}

void Leaderboard::ResetNameEntry()
{
	letterCount = 0;
	name[0] = '\0';
	mouseOnText = false;
	framesCounter = 0;
	enteringName = false;
}
