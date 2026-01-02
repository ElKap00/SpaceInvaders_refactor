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
};

// TODO: write Game constructor for proper initialization
struct Game
{
	State gameState_ = {};
	int score_ = 0;
	int wallCount_ = 5;
	bool isNewHighScore_ = false;

	// Aliens shooting
	float shootTimerSeconds_ = 0;

	// Entity Storage and Resources
	Resources resources_;
	Player player_;
	std::vector<Projectile> projectiles_;
	std::vector<Wall> walls_;
	std::vector<Alien> aliens_;
	AlienFormation alienFormation_;
	Leaderboard leaderboard_;
	Background background_;

	void start();
	void end();

	void resume();
	void launch();

	void update();
	void render();

	void createAlienFormation();

	bool checkCollision(Vector2 circlePos, float circleRadius, Vector2 lineTop, Vector2 lineBottom);
};