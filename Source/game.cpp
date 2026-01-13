#include "game.h"
#include <chrono>
#include <fstream>
#include <iostream>
#include <thread>
#include <vector>
#include <random>
#include <cassert>

void Game::start()
{
	resetScore();
	resetLives();
	createWalls();
	createAlienFormation();
	gameState_ = State::GAMEPLAY;
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
	gameState_ = State::ENDSCREEN;
}

void Game::resume() noexcept
{
	leaderboard_.resetNameEntry();
	gameState_ = State::STARTSCREEN;
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

void Game::draw()
{
	BeginDrawing();
	ClearBackground(BLACK);
	render();
	EndDrawing();
}

void Game::createAlienFormation()
{
	for (int row = 0; row < alienFormation_.formationHeight_; row++) {
		for (int col = 0; col < alienFormation_.formationWidth_; col++) {
			const Vector2 alienPosition = { alienFormation_.position_.x + (static_cast<float>(col) * static_cast<float>(alienFormation_.alienSpacing_)),
											alienFormation_.position_.y + (static_cast<float>(row) * static_cast<float>(alienFormation_.alienSpacing_)) };			
			aliens_.emplace_back(alienPosition);
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

void Game::renderGamePlay() noexcept
{
	background_.render();
	player_.render(resources_.shipTextures_[player_.activeTexture_]);
	renderUI();
	renderRange<Projectile>(playerProjectiles_, resources_.laserTexture_);
	renderRange<Projectile>(alienProjectiles_, resources_.laserTexture_);
	renderRange<Wall>(walls_, resources_.barrierTexture_);
	renderRange<Alien>(aliens_, resources_.alienTexture_);
	
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
	if (player_.lives_ < 1) //TODO: consider making a "isAlive()" function on player
	{
		end();
	}

	player_.update();
	updateAliens(); //TODO: consider making this a generic "update(Range)", "update(begin, end)"

	if (aliens_.size() < 1) //TODO: empty()
	{
		createAlienFormation();
	}	

	updateRange<Projectile>(playerProjectiles_);
	updateRange<Projectile>(alienProjectiles_);
	updateRange<Wall>(walls_);
	background_.updateWithPlayerPosition(player_.getPositionX());

	playerShoot();
	aliensShoot();
	checkCollisions();
	removeInactiveEntities();
}

void Game::renderEndScreen() noexcept
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

template<typename T>
void Game::renderRange(std::span<const T> container, const Texture2D& texture) const noexcept
{
	for (const auto& entity : container)
	{
		entity.render(texture);
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
	DrawRectangleLinesEx(player_.collisionBox_, 2.0f, GREEN);

	for (const auto& alien : aliens_)
	{
		DrawRectangleLinesEx(alien.collisionBox_, 2.0f, RED);
	}

	for (const auto& wall : walls_)
	{
		DrawRectangleLinesEx(wall.collisionBox_, 2.0f, BLUE);
	}

	for (const auto& projectile : playerProjectiles_)
	{
		DrawRectangleLinesEx(projectile.collisionBox_, 2.0f, YELLOW);
	}

	for (const auto& projectile : alienProjectiles_)
	{
		DrawRectangleLinesEx(projectile.collisionBox_, 2.0f, ORANGE);
	}
}

void Game::resetScore() noexcept
{
	score_ = 0;
}

void Game::resetLives() noexcept
{
	player_.lives_ = 3;
}

void Game::createWalls()
{
	const float wall_distance = window_.width_ / (wallCount_ + 1);
	for (int i = 0; i < wallCount_; i++)
	{
		const Wall newWall{ {wall_distance * (i + 1), window_.height_ - 250.0f } };
		walls_.push_back(newWall);
	}
}

template<typename T>
void Game::updateRange(std::span<const T>& container) noexcept
{
	for (auto& entity : container)
	{
		entity.update();
	}
}

void Game::updateAliens()
{
	for (auto& alien : aliens_)
	{
		alien.update();

		if (alien.position_.y >= player_.position_.y - 50.0f) //TODO: consider refactoring complex tests into named functions. "isBehindPlayer(const Alien&)"
		{			//TODO: or maybe: player.getBottom(). alien.bottom() > player.bottom()
			end();
		}
	}
}

void Game::aliensShoot()
{
	alienFormation_.shootTimerSeconds_ += 1.0f;
	if (alienFormation_.shootTimerSeconds_ < 60.0f)
	{
		return;
	}
	if (aliens_.empty())
	{
		return;
	}

	const Alien& shootingAlien = selectRandomAlien();

	const Vector2 projectilePosition = { shootingAlien.position_.x, shootingAlien.position_.y + 40.0f };
	const Projectile newProjectile(projectilePosition, -15);
	alienProjectiles_.push_back(newProjectile);
	alienFormation_.shootTimerSeconds_ = 0.0f;
}

Alien& Game::selectRandomAlien()
{
	assert(aliens_.empty() == false);
	static std::default_random_engine rng(std::random_device{}());
	std::uniform_int_distribution<size_t> dist(0, aliens_.size() - 1);
	const size_t randomAlienIndex = dist(rng);
	assert(randomAlienIndex < aliens_.size());
	//TODO: feel free to silence the static analyzer here. You are guarantueed that index is in range
	return aliens_[randomAlienIndex];
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
	auto removeInactive = [](auto& container) {
		container.erase(
			std::remove_if(container.begin(), container.end(),
				[](const auto& entity) { return !entity.isActive_; }),
			container.end()
		);
	};

	removeInactive(playerProjectiles_);
	removeInactive(alienProjectiles_);
	removeInactive(aliens_);
	removeInactive(walls_);
}

void Game::checkCollisions() noexcept
{
	for (auto& projectile : playerProjectiles_)
	{
		checkAlienCollision(projectile);
		checkWallCollision(projectile);
	}

	for (auto& projectile : alienProjectiles_)
	{
		checkPlayerCollision(projectile);
		checkWallCollision(projectile);
	}
}

void Game::checkWallCollision(Projectile& projectile) noexcept 
{
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

void Game::checkPlayerCollision(Projectile& projectile) noexcept 
{
	if (CheckCollisionRecs(projectile.collisionBox_, player_.collisionBox_))
	{
		projectile.setActive(false);
		player_.lives_ -= 1;
	}
}

void Game::checkAlienCollision(Projectile& projectile) noexcept 
{
	for (auto& alien : aliens_)
	{
		if (CheckCollisionRecs(projectile.collisionBox_, alien.collisionBox_))
		{
			projectile.setActive(false);
			alien.isActive_ =false;
			score_ += 100;
			break;
		}
	}
}