#pragma once
#include "raylib_wrapper.h"
#include <vector>
#include "resources.h"
#include <string>
#include "player.h"
#include "background.h"
#include "entity_type.h"
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
	int formationX_ = 100;
	int formationY_ = 50;
	float shootTimerSeconds_ = 0.0f;
};

class Game
{
private:
	State gameState_ = State::STARTSCREEN;
	int score_ = 0;
	int wallCount_ = 5;
	bool isNewHighScore_ = false;

	const float windowHeight_ = GetScreenHeightF();
	const float windowWidth_ = GetScreenWidthF();

	// TODO: consider EntityManager class for better organization
	// Entity Storage and Resources
	Resources resources_ = Resources{};
	Player player_{windowWidth_/2};
	std::vector<Projectile> projectiles_;
	std::vector<Wall> walls_;
	std::vector<Alien> aliens_;
	AlienFormation alienFormation_ = AlienFormation{};
	Leaderboard leaderboard_ = Leaderboard{};
	Background background_ = Background{};

public:
	Game() = default;
	~Game() = default;

	void setGameState(State state) noexcept;
	void setPlayer(Player player) noexcept;
	void setBackground(Background background);

	Player& getPlayer() noexcept;
	State getGameState() const noexcept;

	void update();
	void render();

private:
	void start();
	void end();
	void resume();

	void renderStartScreen() noexcept;
	void updateStartScreen();
	void renderGamePlay();
	void updateGamePlay();
	void renderEndScreen();
	void updateEndScreen();

	void renderProjectiles();
	void renderWalls();
	void renderAliens();
	void renderUI() noexcept;

	void createWalls();
	void createAlienFormation();
	void updateAliens();

	void aliensShoot();
	void playerShoot();

	void checkCollisions();
	bool doCollide(Vector2 circlePos, float circleRadius, Vector2 lineTop, Vector2 lineBottom);

	void resetScore() noexcept;
	void removeInactiveEntities() noexcept;
};