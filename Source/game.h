#pragma once
#include "raylib_wrapper.h"
#include <vector>
#include "resources.h"
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

struct AlienFormation 
{
	int formationWidth_ = 8;
	int formationHeight_ = 5;
	int alienSpacing_ = 80;
	Vector2 position_ = { 550.0f, 50.0f };
	float shootTimerSeconds_ = 0.0f;
};

struct Window
{
	const float height_ = 1080.0f;
	const float width_ = 1920.0f;
	const std::string title_ = "SPACE INVADERS";

	Window() noexcept
	{
		InitWindow(static_cast<int>(width_), static_cast<int>(height_), title_.c_str());
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
	const Window window_{};

	State gameState_ = State::STARTSCREEN;
	int score_ = 0;
	int wallCount_ = 5;
	bool isNewHighScore_ = false;
	bool debugCollisionBoxes_ = false; // Toggle for debug rendering

	// TODO: consider EntityManager class for better organization
	// Entity Storage and Resources
	Resources resources_{};
	Player player_{};
	std::vector<Projectile> playerProjectiles_;
	std::vector<Projectile> alienProjectiles_;
	std::vector<Wall> walls_;
	std::vector<Alien> aliens_;
	AlienFormation alienFormation_{};
	Leaderboard leaderboard_{};
	Background background_{};

public:
	Game() = default;
	~Game() = default;

	void update();
	void render();
	void draw();

private:
	void start();
	void end();
	void resume();

	void renderStartScreen() noexcept;
	void updateStartScreen();
	void renderGamePlay() noexcept;
	void updateGamePlay();
	void renderEndScreen();
	void updateEndScreen();

	void renderProjectiles() noexcept;
	void renderWalls() noexcept;
	void renderAliens() noexcept;
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