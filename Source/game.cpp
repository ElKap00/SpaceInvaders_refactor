#include "game.h"
#include <chrono>
#include <fstream>
#include <iostream>
#include <thread>
#include <vector>
#include <random>
#include <cassert>

void Game::run()
{
	while (!WindowShouldClose())
	{
		update();
		render();
	}
}

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
		throw std::logic_error("Invalid game state in Game::update().");
		break;
	}
}

void Game::render() noexcept
{
	BeginDrawing();
	ClearBackground(BLACK);

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
		break;
	}

	EndDrawing();
}

void Game::createAlienFormation()
{
	for (int row = 0; row < alienFormation_.formationHeight_; row++) {
		for (int col = 0; col < alienFormation_.formationWidth_; col++) {
			const Vector2 alienPosition = { alienFormation_.initialPosition_.x + (static_cast<float>(col) * static_cast<float>(alienFormation_.alienSpacing_)),
											alienFormation_.initialPosition_.y + (static_cast<float>(row) * static_cast<float>(alienFormation_.alienSpacing_)) };			
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
	player_.render();
	renderUI();
	render<Projectile>(playerProjectiles_);
	render<Projectile>(alienProjectiles_);
	render<Wall>(walls_);
	render<Alien>(aliens_);
	
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
	if (!player_.isAlive())
	{
		end();
	}

	player_.update();
	updateAliens();

	if (aliens_.empty())
	{
		createAlienFormation();
	}	

	update<Projectile>(playerProjectiles_);
	update<Projectile>(alienProjectiles_);
	update<Wall>(walls_);
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

void Game::renderUI() const noexcept
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

	std::for_each(aliens_.begin(), aliens_.end(),
		[](const auto& alien) noexcept {
			DrawRectangleLinesEx(alien.collisionBox_, 2.0f, RED);
		});

	std::for_each(walls_.begin(), walls_.end(),
		[](const auto& wall) noexcept {
			DrawRectangleLinesEx(wall.collisionBox_, 2.0f, BLUE);
		});

	std::for_each(playerProjectiles_.begin(), playerProjectiles_.end(),
		[](const auto& projectile) noexcept {
			DrawRectangleLinesEx(projectile.getCollisionBox(), 2.0f, YELLOW);
		});

	std::for_each(alienProjectiles_.begin(), alienProjectiles_.end(),
		[](const auto& projectile) noexcept {
			DrawRectangleLinesEx(projectile.getCollisionBox(), 2.0f, ORANGE);
		});
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
		walls_.emplace_back(Vector2{ wall_distance * (i + 1), window_.height_ - 250.0f });
	}
}

void Game::updateAliens()
{
	//TODO: consider refactoring complex tests into named functions. "isBehindPlayer(const Alien&)"
	//TODO: or maybe: player.getBottom(). alien.bottom() > player.bottom()
	std::for_each(aliens_.begin(), aliens_.end(),
		[](auto& alien) noexcept { alien.update(); });

	if (std::any_of(aliens_.begin(), aliens_.end(),
		[this](const auto& alien) {
			return alien.position_.y >= player_.position_.y - 50.0f;
		}))
	{
		end();
	}
}

void Game::aliensShoot()
{
	alienShootCooldown_ += 1.0f;
	if (alienShootCooldown_ < 60.0f)
	{
		return;
	}
	if (aliens_.empty())
	{
		return;
	}

	const Alien& shootingAlien = selectRandomAlien();

	const Vector2 projectilePosition = { shootingAlien.position_.x, shootingAlien.position_.y + 40.0f };
	alienProjectiles_.emplace_back(projectilePosition, -15);
	alienShootCooldown_ = 0.0f;
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
		playerProjectiles_.emplace_back(projectileSpawnPos);
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
	std::for_each(playerProjectiles_.begin(), playerProjectiles_.end(),
		[this](auto& projectile) noexcept {
			checkAlienCollision(projectile);
			checkWallCollision(projectile);
		});

	std::for_each(alienProjectiles_.begin(), alienProjectiles_.end(),
		[this](auto& projectile) noexcept {
			checkPlayerCollision(projectile);
			checkWallCollision(projectile);
		});
}

void Game::checkWallCollision(Projectile& projectile) noexcept 
{
	auto it = std::find_if(walls_.begin(), walls_.end(),
		[&projectile](auto& wall) noexcept {
			return CheckCollisionRecs(projectile.getCollisionBox(), wall.collisionBox_);
		});

	if (it != walls_.end())
	{
		projectile.isActive_ = false;
		it->health_ -= 1;
	}
}

void Game::checkPlayerCollision(Projectile& projectile) noexcept 
{
	if (CheckCollisionRecs(projectile.getCollisionBox(), player_.collisionBox_))
	{
		projectile.isActive_ = false;
		player_.lives_ -= 1;
	}
}

void Game::checkAlienCollision(Projectile& projectile) noexcept 
{
	auto it = std::find_if(aliens_.begin(), aliens_.end(),
		[&projectile](auto& alien) noexcept {
			return CheckCollisionRecs(projectile.getCollisionBox(), alien.collisionBox_);
		});

	if (it != aliens_.end())
	{
		projectile.isActive_ = false;
		it->isActive_ = false;
		score_ += 100;
	}
}

// Helper methods
template<typename T>
void Game::render(std::span<T> container) noexcept
{
	for (const auto& entity : container)
	{
		entity.render();
	}
}

template<typename T>
void Game::update(std::span<T> container) noexcept
{
	for (auto& entity : container)
	{
		entity.update();
	}
}