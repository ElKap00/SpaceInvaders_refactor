#include "game.h"
#include <iostream>
#include <vector>
#include <chrono>
#include <thread>
#include <fstream>


// MATH FUNCTIONS
float lineLength(Vector2 A, Vector2 B) //Uses pythagoras to calculate the length of a line
{
	// TODO: make variable const
	float length = sqrtf(pow(B.x - A.x, 2) + pow(B.y - A.y, 2));

	return length;
}

bool pointInCircle(Vector2 circlePos, float radius, Vector2 point) // Uses pythagoras to calculate if a point is within a circle or not
{
	// TODO:  make variables const
	float distanceToCentre = lineLength(circlePos, point);

	if (distanceToCentre < radius)
	{
		return true;
	}
	else
	{
		return false;
	}
}

bool PointOnLine(Vector2 lineStart, Vector2 lineEnd, Vector2 point, float buffer = 0.1f)
{
	const float length = lineLength(lineStart, lineEnd);
	const float distanceToStart = lineLength(lineStart, point);
	const float distanceToEnd = lineLength(lineEnd, point);
	const float combinedDistance = distanceToStart + distanceToEnd;

	return (combinedDistance >= length - buffer && combinedDistance <= length + buffer);
}


// TODO: break up into smaller utility functions if possible
void Game::start()
{
	createWalls();

	// TODO: remove multi-step initialization
	//creating player
	Player newPlayer;
	player_ = newPlayer;
	player_.initialize();

	//creating aliens
	createAlienFormation();
	

	//creating background
	Background newBackground;
	newBackground.initialize(600);
	background_ = newBackground;

	//reset score
	score_ = 0;

	gameState_ = State::GAMEPLAY;

}
void Game::createWalls()
{
	// TODO: fix C-style casts, mark variables as const
	const float window_width = (float)GetScreenWidth();
	const float window_height = (float)GetScreenHeight();
	const float wall_distance = window_width / (wallCount_ + 1);
	for (int i = 0; i < wallCount_; i++)
	{
		Wall newWall{ {wall_distance * (i + 1), window_height - 250 } };
		walls_.push_back(newWall);
	}
}

void Game::end()
{
	//SAVE SCORE AND UPDATE SCOREBOARD
	projectiles_.clear();
	walls_.clear();
	aliens_.clear();
	isNewHighScore_ = leaderboard_.isNewHighScore(score_);
	if (isNewHighScore_)
	{
		leaderboard_.startNameEntry();
	}
	gameState_ = State::ENDSCREEN;
}

void Game::resume()
{
	leaderboard_.saveLeaderboard();
	leaderboard_.resetNameEntry();
	gameState_ = State::STARTSCREEN;
}

void Game::launch()
{
	//LOAD SOME RESOURCES HERE
	resources_.load();
}

// TODO: break up this huge function into smaller functions
void Game::update()
{
	switch (gameState_)
	{
	case State::STARTSCREEN:
		//Code 
		if (IsKeyReleased(KEY_SPACE))
		{
			start();
		}

		break;
	case State::GAMEPLAY:
		//Code
		if (IsKeyReleased(KEY_Q))
		{
			end();
		}

		//Update Player
		player_.update();
		
		//Update Aliens and Check if they are past player
		for (int i = 0; i < aliens_.size(); i++)
		{
			aliens_[i].update(); 

			if (aliens_[i].position_.y > GetScreenHeight() - player_.height_)
			{
				end();
			}
		}

		//End game if player dies
		if (player_.lives_ < 1)
		{
			end();
		}

		//Spawn new aliens if aliens run out
		if (aliens_.size() < 1)
		{
			createAlienFormation();
		}

		// Update background with offset
		background_.playerPos_ = { player_.positionX_, (float)player_.height_ };
		background_.cornerPos_ = { 0, (float)player_.height_ };
		background_.offset_ = lineLength(background_.playerPos_, background_.cornerPos_) * -1;
		background_.update(background_.offset_ / 15);

		// TODO: use ranged for-loops
		//UPDATE PROJECTILE
		for (int i = 0; i < projectiles_.size(); i++)
		{
			projectiles_[i].update();
		}
		//UPDATE PROJECTILE
		for (int i = 0; i < walls_.size(); i++)
		{
			walls_[i].update();
		}

		// TODO: improve nested loops here
		// TODO: check logic
		//CHECK ALL COLLISONS HERE
		for (int i = 0; i < projectiles_.size(); i++)
		{
			if (projectiles_[i].type_ == EntityType::PLAYER_PROJECTILE)
			{
				for (int a = 0; a < aliens_.size(); a++)
				{
					if (checkCollision(aliens_[a].position_, aliens_[a].radius_, projectiles_[i].lineStart_, projectiles_[i].lineEnd_))
					{
						// Kill!
						std::cout << "Hit! \n";
						// Set them as inactive, will be killed later
						projectiles_[i].active_ = false;
						aliens_[a].isActive_ = false;
						score_ += 100;
					}
				}
			}

			//ENEMY PROJECTILES HERE
			for (int i = 0; i < projectiles_.size(); i++)
			{
				if (projectiles_[i].type_ == EntityType::ENEMY_PROJECTILE)
				{
					if (checkCollision({player_.positionX_, GetScreenHeight() - player_.height_ }, player_.radius_, projectiles_[i].lineStart_, projectiles_[i].lineEnd_))
					{
						std::cout << "dead!\n"; 
						projectiles_[i].active_ = false; 
						player_.lives_ -= 1; 
					}
				}
			}


			for (int b = 0; b < walls_.size(); b++)
			{
				if (checkCollision(walls_[b].position_, walls_[b].radius_, projectiles_[i].lineStart_, projectiles_[i].lineEnd_))
				{
					// Kill!
					std::cout << "Hit! \n";
					// Set them as inactive, will be killed later
					projectiles_[i].active_ = false;
					walls_[b].health_ -= 1;
				}
			}
		}

		//MAKE PROJECTILE
		if (IsKeyPressed(KEY_SPACE))
		{
			float window_height = (float)GetScreenHeight();
			Projectile newProjectile;
			newProjectile.position_.x = player_.positionX_;
			newProjectile.position_.y = window_height - 130;
			newProjectile.type_ = EntityType::PLAYER_PROJECTILE;
			projectiles_.push_back(newProjectile);
		}

		//Aliens Shooting
		shootTimerSeconds_ += 1;
		if (shootTimerSeconds_ > 59) //once per second
		{
			int randomAlienIndex = 0;

			if (aliens_.size() > 1)
			{
				randomAlienIndex = rand() % aliens_.size();
			}

			Projectile newProjectile;
			newProjectile.position_ = aliens_[randomAlienIndex].position_;
			newProjectile.position_.y += 40;
			newProjectile.speed_ = -15;
			newProjectile.type_ = EntityType::ENEMY_PROJECTILE;
			projectiles_.push_back(newProjectile);
			shootTimerSeconds_ = 0;
		}

		// REMOVE INACTIVE/DEAD ENTITIES
		for (int i = 0; i < projectiles_.size(); i++)
		{
			if (projectiles_[i].active_ == false)
			{
				projectiles_.erase(projectiles_.begin() + i);
				// Prevent the loop from skipping an instance because of index changes, since all insances after
				// the killed objects are moved down in index. This is the same for all loops with similar function
				i--;
			}
		}
		for (int i = 0; i < aliens_.size(); i++)
		{
			if (aliens_[i].isActive_ == false)
			{
				aliens_.erase(aliens_.begin() + i);
				i--;
			}
		}
		for (int i = 0; i < walls_.size(); i++)
		{
			if (walls_[i].isActive() == false)
			{
				walls_.erase(walls_.begin() + i);
				i--;
			}
		}
	break;
	case State::ENDSCREEN:
		//Exit endscreen
 		if (IsKeyReleased(KEY_ENTER) && !isNewHighScore_)
		{
			resume();
		}

		if (isNewHighScore_)
		{
			leaderboard_.updateHighScoreNameEntry();
			
			// Check if name entry is complete
			if (!leaderboard_.isEnteringName())
			{
				// Insert the high score with the player's name
				leaderboard_.insertNewHighScore(leaderboard_.getEnteredName(), score_);
				isNewHighScore_ = false;
			}
		}
		break;
	default:
		//SHOULD NOT HAPPEN
		break;
	}
}

// TODO: break up into smaller functions
void Game::render()
{
	switch (gameState_)
	{
	case State::STARTSCREEN:
		//Code
		DrawText("SPACE INVADERS", 200, 100, 160, YELLOW);

		DrawText("PRESS SPACE TO BEGIN", 200, 350, 40, YELLOW);

		break;
	case State::GAMEPLAY:

		//background render LEAVE THIS AT TOP
		background_.render();

		//DrawText("GAMEPLAY", 50, 30, 40, YELLOW);
		DrawText(TextFormat("Score: %i", score_), 50, 20, 40, YELLOW);
		DrawText(TextFormat("Lives: %i", player_.lives_), 50, 70, 40, YELLOW);

		//player rendering 
		player_.render(resources_.shipTextures_[player_.activeTexture_]);

		//projectile rendering
		for (int i = 0; i < projectiles_.size(); i++)
		{
			projectiles_[i].render(resources_.laserTexture_);
		}

		// wall rendering 
		for (int i = 0; i < walls_.size(); i++)
		{
			walls_[i].render(resources_.barrierTexture_); 
		}

		//alien rendering  
		for (int i = 0; i < aliens_.size(); i++)
		{
			aliens_[i].render(resources_.alienTexture_);
		}

		break;
	case State::ENDSCREEN:

		if (isNewHighScore_)
		{
			leaderboard_.renderHighScoreEntry();
			leaderboard_.renderHighScoreNameInput();
		}
		else {
			leaderboard_.renderLeaderboard();
		}

		break;
	default:
		//SHOULD NOT HAPPEN
		break;
	}
}

void Game::createAlienFormation()
{
	for (int row = 0; row < alienFormation_.formationHeight_; row++) {
		for (int col = 0; col < alienFormation_.formationWidth_; col++) {
			// TODO: remove multi-step initialization
			Alien newAlien = Alien();
			newAlien.isActive_ = true;
			newAlien.position_.x = alienFormation_.formationX_ + 450 + (col * alienFormation_.alienSpacing_);
			newAlien.position_.y = alienFormation_.formationY_ + (row * alienFormation_.alienSpacing_);
			aliens_.push_back(newAlien);
			std::cout << "Find Alien -X:" << newAlien.position_.x << std::endl;
			std::cout << "Find Alien -Y:" << newAlien.position_.y << std::endl;
		}
	}
}

bool Game::checkCollision(Vector2 circlePos, float circleRadius, Vector2 lineStart, Vector2 lineEnd)
{
	if (pointInCircle(circlePos, circleRadius, lineStart) || pointInCircle(circlePos, circleRadius, lineEnd))
	{
		return true;
	}

	const float lineLen = lineLength(lineStart, lineEnd);
	
	const float dotProduct = (((circlePos.x - lineStart.x) * (lineEnd.x - lineStart.x)) 
						+ ((circlePos.y - lineStart.y) * (lineEnd.y - lineStart.y))) / pow(lineLen, 2);

	const Vector2 closestPoint = {
		lineStart.x + (dotProduct * (lineEnd.x - lineStart.x)),
		lineStart.y + (dotProduct * (lineEnd.y - lineStart.y))
	};

	if (PointOnLine(lineStart, lineEnd, closestPoint))
	{
		const float closeToCentre = lineLength(circlePos, closestPoint);

		if (closeToCentre < circleRadius)
		{
			return true;
		}
	}
	return false;
}

void Game::createWalls()
{
	// TODO: fix C-style casts, mark variables as const
	const float window_width = (float)GetScreenWidth();
	const float window_height = (float)GetScreenHeight();
	const float wall_distance = window_width / (wallCount_ + 1);
	for (int i = 0; i < wallCount_; i++)
	{
		Wall newWall{ {wall_distance * (i + 1), window_height - 250 } };
		walls_.push_back(newWall);
	}
}