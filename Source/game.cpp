#include "game.h"
#include <chrono>
#include <fstream>
#include <iostream>
#include <thread>
#include <vector>

void Game::setGameState(State state) noexcept { gameState_ = state; }

void Game::setPlayer(Player player) noexcept { player_ = player; }

void Game::setBackground(Background background) { background_ = background; }

Player& Game::getPlayer() noexcept { return player_; }

State Game::getGameState() const noexcept { return gameState_; }


void Game::start()
{
	resetScore();
	createWalls();
	createAlienFormation();
	setPlayer(Player{});
	setGameState(State::GAMEPLAY);
}

void Game::end()
{
	alienProjectiles_.clear();
	playerProjectiles_.clear();
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
			const Vector2 alienPosition = { alienFormation_.formationX_ + 450.0f + (static_cast<float>(col) * static_cast<float>(alienFormation_.alienSpacing_)),
											alienFormation_.formationY_ + (static_cast<float>(row) * static_cast<float>(alienFormation_.alienSpacing_)) };
			const Alien newAlien = Alien(alienPosition);
			aliens_.push_back(newAlien);
		}
	}
}

void Game::renderStartScreen() noexcept
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
	
	if (debugCollisionBoxes_)
	{
		renderCollisionBoxes();
	}
}

void Game::updateGamePlay()
{
	// Toggle debug collision boxes with 'D' key
	if (IsKeyReleased(KEY_D))
	{
		debugCollisionBoxes_ = !debugCollisionBoxes_;
	}

	if (IsKeyReleased(KEY_Q))
	{
		end();
	}
	if (getPlayer().getLives() < 1)
	{
		end();
	}

	getPlayer().update();
	updateAliens(); //TODO: consider making this a generic "update(Range)", "update(begin, end)"

	if (aliens_.size() < 1)
	{
		createAlienFormation();
	}
	

	background_.updateWithPlayerPosition(player_.getPositionX(), player_.height_);

	for (auto& projectile : playerProjectiles_)
	{
		projectile.update();
	}

	for (auto& projectile : alienProjectiles_)
	{
		projectile.update();
	}

	for (auto& wall : walls_)
	{
		wall.update();
	}
	playerShoot();
	aliensShoot();
	checkCollisions();
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
	if (IsKeyReleased(KEY_ENTER) && !isNewHighScore_)
	{
		resume();
	}

	if (isNewHighScore_)
	{
		leaderboard_.updateHighScoreNameEntry();

		if (!leaderboard_.getIsEnteringName())
		{
			leaderboard_.insertNewHighScore(score_);
			isNewHighScore_ = false;
		}
	}
}

void Game::renderProjectiles()
{
	for (auto& projectile : playerProjectiles_)
	{
		projectile.render(resources_.laserTexture_);
	}
	for (auto& projectile : alienProjectiles_)
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

void Game::renderUI() noexcept
{
	DrawText(TextFormat("Score: %i", score_), 50, 20, 40, YELLOW);
	DrawText(TextFormat("Lives: %i", player_.lives_), 50, 70, 40, YELLOW);
	
	if (debugCollisionBoxes_)
	{
		DrawText("DEBUG: Collision Boxes ON (Press D to toggle)", 50, 120, 20, GREEN);
	}
}

void Game::renderCollisionBoxes() noexcept
{
	// Draw player collision box
	DrawRectangleLinesEx(player_.collisionBox_, 2.0f, GREEN);

	// Draw alien collision boxes
	for (const auto& alien : aliens_)
	{
		DrawRectangleLinesEx(alien.collisionBox_, 2.0f, RED);
	}

	// Draw wall collision boxes
	for (const auto& wall : walls_)
	{
		DrawRectangleLinesEx(wall.collisionBox_, 2.0f, BLUE);
	}

	// Draw player projectile collision boxes
	for (const auto& projectile : playerProjectiles_)
	{
		DrawRectangleLinesEx(projectile.collisionBox_, 2.0f, YELLOW);
	}

	// Draw alien projectile collision boxes
	for (const auto& projectile : alienProjectiles_)
	{
		DrawRectangleLinesEx(projectile.collisionBox_, 2.0f, ORANGE);
	}
}

void Game::resetScore() noexcept
{
	score_ = 0;
}

void Game::createWalls()
{
	const float wall_distance = windowWidth_ / (wallCount_ + 1);
	for (int i = 0; i < wallCount_; i++)
	{
		const Wall newWall{ {wall_distance * (i + 1), windowHeight_ - 250.0f } };
		walls_.push_back(newWall);
	}
}

void Game::updateAliens()
{
	for (auto& alien : aliens_)
	{
		alien.update();

		if (alien.position_.y > windowHeight_ - player_.height_)
		{
			end();
		}
	}
}

void Game::aliensShoot()
{
	alienFormation_.shootTimerSeconds_ += 1.0f;
	if (alienFormation_.shootTimerSeconds_ > 59.0f) //once per second
	{
		int randomAlienIndex = 0;

		if (aliens_.size() > 1)
		{
			randomAlienIndex = rand() % aliens_.size();
		}

		const Vector2 projectilePosition = { aliens_[randomAlienIndex].position_.x, aliens_[randomAlienIndex].position_.y + 40.0f };
		const Projectile newProjectile(projectilePosition, -15);
		alienProjectiles_.push_back(newProjectile);
		alienFormation_.shootTimerSeconds_ = 0.0f;
	}
}

void Game::playerShoot()
{
	if (IsKeyPressed(KEY_SPACE))
	{
		const Vector2 projectileSpawnPos = {
			player_.position_.x,
			player_.position_.y - 50.0f  // Spawn from top of player collision box
		};
		const Projectile newProjectile(projectileSpawnPos);
		playerProjectiles_.push_back(newProjectile);
	}
}

void Game::removeInactiveEntities() noexcept
{
	playerProjectiles_.erase(
		std::remove_if(playerProjectiles_.begin(), playerProjectiles_.end(),
			[](const Projectile& projectile) { return !projectile.isActive_; }),
		playerProjectiles_.end()
	);

	alienProjectiles_.erase(
		std::remove_if(alienProjectiles_.begin(), alienProjectiles_.end(),
			[](const Projectile& projectile) { return !projectile.isActive_; }),
		alienProjectiles_.end()
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
	checkPlayerProjectileCollisions();
	checkAlienProjectileCollisions();
}

void Game::checkPlayerProjectileCollisions()
{
	for (auto& projectile : playerProjectiles_)
	{
		// Check collision with aliens
		for (auto& alien : aliens_)
		{
			if (CheckCollisionRecs(projectile.collisionBox_, alien.collisionBox_))
			{
				projectile.setActive(false);
				alien.setActive(false);
				score_ += 100;
				break;
			}
		}

		// Check collision with walls
		for (auto& wall : walls_)
		{
			if (CheckCollisionRecs(projectile.collisionBox_, wall.collisionBox_))
			{
				projectile.setActive(false);
				wall.health_ -= 1;
				break;
			}
		}
	}
}

void Game::checkAlienProjectileCollisions()
{
	for (auto& projectile : alienProjectiles_)
	{
		// Check collision with player
		if (CheckCollisionRecs(projectile.collisionBox_, player_.collisionBox_))
		{
			projectile.setActive(false);
			player_.lives_ -= 1;
		}

		// Check collision with walls
		for (auto& wall : walls_)
		{
			if (CheckCollisionRecs(projectile.collisionBox_, wall.collisionBox_))
			{
				projectile.setActive(false);
				wall.health_ -= 1;
				break;
			}
		}
	}
}