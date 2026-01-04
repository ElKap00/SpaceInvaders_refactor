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

void Game::setGameState(State state) { gameState_ = state; }

void Game::setPlayer(Player player) { player_ = player; }

void Game::setBackground(Background background) { background_ = background; }

Player& Game::getPlayer() noexcept { return player_; }

State Game::getGameState() const noexcept { return gameState_; }

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
	leaderboard_.resetNameEntry();
	setGameState(State::STARTSCREEN);
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
		updateStartScreen();
		break;

	case State::GAMEPLAY:
		updateGamePlay();
		break;

	case State::ENDSCREEN:
		updateEndScreen();
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
		renderGamePlay();
		break;

	case State::ENDSCREEN:
		renderEndScreen();
		break;

	default:
		// TODO: add exception handling
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

void Game::updateStartScreen()
{
	if (IsKeyReleased(KEY_SPACE))
	{
		start();
	}
}

void Game::renderGamePlay()
{
	background_.render();
	renderUI();
	getPlayer().render(resources_.shipTextures_[player_.activeTexture_]);
	renderProjectiles();
	renderWalls();
	renderAliens();
}

void Game::updateGamePlay()
{
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

	// TODO: separate background update logic
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

	playerShoot();
	aliensShoot();
	removeInactiveEntities();
}

void Game::renderEndScreen()
{
	if (isNewHighScore_)
	{
		leaderboard_.renderHighScoreEntry();
		leaderboard_.renderHighScoreNameInput();
	}
	else {
		leaderboard_.renderLeaderboard();
	}
}

void Game::updateEndScreen()
{
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

		const Vector2 projectilePosition = { aliens_[randomAlienIndex].position_.x, aliens_[randomAlienIndex].position_.y + 40 };
		const Projectile newProjectile(projectilePosition, -15, EntityType::ENEMY_PROJECTILE);
		projectiles_.push_back(newProjectile);
		shootTimerSeconds_ = 0;
	}
}

// TODO: move this to Projectile class?
void Game::playerShoot()
{
	if (IsKeyPressed(KEY_SPACE))
	{
		const float window_height = (float)GetScreenHeight();
		const Projectile newProjectile({ player_.positionX_, window_height - 130 }, EntityType::PLAYER_PROJECTILE);
		projectiles_.push_back(newProjectile);
	}
}

void Game::removeInactiveEntities()
{
	projectiles_.erase(
		std::remove_if(projectiles_.begin(), projectiles_.end(),
			[](const Projectile& projectile) { return !projectile.isActive_; }),
		projectiles_.end()
	);

	aliens_.erase(
		std::remove_if(aliens_.begin(), aliens_.end(),
			[](const Alien& alien) { return !alien.isActive_; }),
		aliens_.end()
	);

	walls_.erase(
		std::remove_if(walls_.begin(), walls_.end(),
			[](const Wall& wall) { return !wall.isActive(); }),
		walls_.end()
	);
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
					projectile.setActive(false);
					alien.setActive(false);
					score_ += 100;
				}
			}
		}

		if (projectile.type_ == EntityType::ENEMY_PROJECTILE)
		{
			if (doCollide({ player_.positionX_, GetScreenHeight() - player_.height_ }, player_.radius_, projectile.lineStart_, projectile.lineEnd_))
			{
				std::cout << "dead!\n";
				projectile.setActive(false);
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
				projectile.setActive(false);
				wall.health_ -= 1;
			}
		}
	}
}