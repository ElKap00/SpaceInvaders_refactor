#pragma once
#include <span>
#include "raylib_wrapper.h"
#include <vector>
#include "texture_resource.h"
#include <string>
#include "player.h"
#include "background.h"
#include "alien.h"
#include "projectile.h"
#include "wall.h"
#include "leaderboard.h"

enum struct State
{
	STARTSCREEN,
	GAMEPLAY,
	ENDSCREEN
};

struct AlienFormation //TODO: is this a config? it doesn't seem to be an alien formation
{
	int formationWidth_ = 8; //TODO: if these are settings / configs that never change, consider static constexpr
	int formationHeight_ = 5;
	int alienSpacing_ = 80;
	Vector2 position_ = { 550.0f, 50.0f };
	float shootTimerSeconds_ = 0.0f; //possibly move thisout of "config" and into game, it's a game logic thing
};

struct Window
{
	const float height_ = 1080.0f; //TODO: const member is almost always wrong. You meant "static const" or "static constexpr"
	const float width_ = 1920.0f;
	const std::string_view title_ = "SPACE INVADERS";

	Window()
	{
		InitWindow(static_cast<int>(width_), static_cast<int>(height_), title_.data());

		if (!IsWindowReady())
		{
			throw std::runtime_error("Failed to initialize game window.");
		}

		SetTargetFPS(60);
	}

	~Window() noexcept
	{
		CloseWindow();
	}
};

class Game
{
private:
	Window window_{}; //TODO: const members break value semantics. We can't assign to these, can't move from them. Our "Game" no longer acts like a normal value.
	State gameState_ = State::STARTSCREEN;
	int score_ = 0;
	int wallCount_ = 5;
	bool isNewHighScore_ = false;
	bool debugCollisionBoxes_ = false; // Toggle for debug rendering

	// Entity Storage and Resources
	Player player_{};
	std::vector<Projectile> playerProjectiles_;
	std::vector<Projectile> alienProjectiles_;
	std::vector<Wall> walls_;
	std::vector<Alien> aliens_;
	AlienFormation alienFormation_{};
	Leaderboard leaderboard_{};
	Background background_{};

public:
	void run();
	void update();
	void render() noexcept;

private:
	void start();
	void end();
	void resume() noexcept;

	//TODO: consider making these free functions instead of member functions, unless they need access to private members
	template<typename T>
	void update(std::span<T> container) noexcept;
	template<typename T>
	void render(std::span<T> container) noexcept;

	void renderStartScreen() noexcept;
	void updateStartScreen();
	void renderGamePlay() noexcept;
	void updateGamePlay();
	void renderEndScreen() noexcept;
	void updateEndScreen();

	void renderUI() noexcept;
	void renderCollisionBoxes() noexcept; // Debug rendering

	void createWalls();
	void createAlienFormation();

	void updateAliens();

	void aliensShoot();
	Alien& selectRandomAlien();
	void playerShoot();

	void checkCollisions() noexcept;
	void checkWallCollision(Projectile& projectile) noexcept;
	void checkPlayerCollision(Projectile& projectile) noexcept;
	void checkAlienCollision(Projectile& projectile) noexcept;

	void resetScore() noexcept;
	void resetLives() noexcept;
	void removeInactiveEntities() noexcept;
};