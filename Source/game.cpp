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

void Game::setGameState(State state)
{
	gameState_ = state;
}

void Game::setPlayer(Player player)
{
	player_ = player;
}

void Game::setBackground(Background background)
{
	background_ = background;
}

Player& Game::getPlayer() noexcept
{
	return player_;
}

// TODO: break up into smaller utility functions if possible
void Game::start()
{
	resetScore();
	createWalls();
	createAlienFormation();
	setPlayer(Player{});
	setBackground(Background{});
	setGameState(State::GAMEPLAY);
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
	setGameState(State::ENDSCREEN);
}

void Game::resume()
{
	leaderboard_.saveLeaderboard();
	leaderboard_.resetNameEntry();
	setGameState(State::ENDSCREEN);
}

void Game::launch()
{
	resources_.load();
}

// TODO: break up this huge function into smaller functions
void Game::update()
{
	switch (gameState_)
	{
	case State::STARTSCREEN:
		if (IsKeyReleased(KEY_SPACE))
		{
			start();
		}

		break;
	case State::GAMEPLAY:
		if (IsKeyReleased(KEY_Q))
		{
			end();
		}
		if (getPlayer().getLives() < 1)
		{
			end();
		}

		getPlayer().update();
		updateAliens();

		if (aliens_.size() < 1)
		{
			createAlienFormation();
		}

		checkCollisions();

		// Update background with offset
		background_.playerPos_ = { player_.positionX_, (float)player_.height_ };
		background_.cornerPos_ = { 0, (float)player_.height_ };
		background_.offset_ = lineLength(background_.playerPos_, background_.cornerPos_) * -1;
		background_.update(background_.offset_ / 15);

		for (auto& projectile : projectiles_)
		{
			projectile.update();
		}

		for (auto& wall : walls_)
		{
			wall.update();
		}

		makeProjectile();
		aliensShoot();
		removeInactiveEntities();

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

		renderStartScreen();

		break;
	case State::GAMEPLAY:

		background_.render();
		renderUI();
		getPlayer().render(resources_.shipTextures_[player_.activeTexture_]);
		renderProjectiles();
		renderWalls();
		renderAliens();

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
			const Vector2 alienPosition = { alienFormation_.formationX_ + 450 + (col * alienFormation_.alienSpacing_),
									alienFormation_.formationY_ + (row * alienFormation_.alienSpacing_) };
			const Alien newAlien = Alien(alienPosition);
			aliens_.push_back(newAlien);
		}
	}
}

bool Game::doCollide(Vector2 circlePos, float circleRadius, Vector2 lineStart, Vector2 lineEnd)
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

void Game::renderStartScreen()
{
	DrawText("SPACE INVADERS", 200, 100, 160, YELLOW);
	DrawText("PRESS SPACE TO BEGIN", 200, 350, 40, YELLOW);
}
void Game::renderProjectiles()
{
	for (auto& projectile : projectiles_)
	{
		projectile.render(resources_.laserTexture_);
	}
}

void Game::renderWalls()
{
	for (auto& wall : walls_)
	{
		wall.render(resources_.barrierTexture_);
	}
}

void Game::renderAliens()
{
	for (auto& alien : aliens_)
	{
		alien.render(resources_.alienTexture_);
	}
}

void Game::renderUI()
{
	DrawText(TextFormat("Score: %i", score_), 50, 20, 40, YELLOW);
	DrawText(TextFormat("Lives: %i", player_.lives_), 50, 70, 40, YELLOW);
}

void Game::resetScore()
{
	score_ = 0;
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

void Game::updateAliens()
{
	for (auto& alien : aliens_)
	{
		alien.update();

		if (alien.position_.y > GetScreenHeight() - player_.height_)
		{
			end();
		}
	}
}

void Game::aliensShoot()
{
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
}

// TODO: move this to Projectile class?
void Game::makeProjectile()
{
	if (IsKeyPressed(KEY_SPACE))
	{
		float window_height = (float)GetScreenHeight();
		Projectile newProjectile;
		newProjectile.position_.x = player_.positionX_;
		newProjectile.position_.y = window_height - 130;
		newProjectile.type_ = EntityType::PLAYER_PROJECTILE;
		projectiles_.push_back(newProjectile);
	}
}

void Game::removeInactiveEntities()
{
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
}

void Game::checkCollisions()
{
	//CHECK ALL COLLISONS HERE
	for (auto& projectile : projectiles_)
	{
		if (projectile.type_ == EntityType::PLAYER_PROJECTILE)
		{
			for (auto& alien : aliens_)
			{
				if (doCollide(alien.position_, alien.radius_, projectile.lineStart_, projectile.lineEnd_))
				{
					// Kill!
					std::cout << "Hit! \n";
					// Set them as inactive, will be killed later
					projectile.active_ = false;
					alien.isActive_ = false;
					score_ += 100;
				}
			}
		}

		if (projectile.type_ == EntityType::ENEMY_PROJECTILE)
		{
			if (doCollide({ player_.positionX_, GetScreenHeight() - player_.height_ }, player_.radius_, projectile.lineStart_, projectile.lineEnd_))
			{
				std::cout << "dead!\n";
				projectile.active_ = false;
				player_.lives_ -= 1;
			}
		}

		for (auto& wall : walls_)
		{
			if (doCollide(wall.position_, wall.radius_, projectile.lineStart_, projectile.lineEnd_))
			{
				// Kill!
				std::cout << "Hit! \n";
				// Set them as inactive, will be killed later
				projectile.active_ = false;
				wall.health_ -= 1;
			}
		}
	}
}