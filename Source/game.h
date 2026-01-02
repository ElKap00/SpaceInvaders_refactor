#pragma once
#include "raylib_wrapper.h"
#include <vector>
#include "Resources.h"
#include <string>
#include "Player.h"
#include "Background.h"
#include "EntityType.h"
#include "Alien.h"
#include "Projectile.h"
#include "Wall.h"
#include "Leaderboard.h"

enum struct State
{
	STARTSCREEN,
	GAMEPLAY,
	ENDSCREEN
};

struct AlienFormation 
{
	int formationWidth = 8;
	int formationHeight = 5;
	int alienSpacing = 80;
	int formationX = 100;
	int formationY = 50;
};

// TODO: write Game constructor for proper initialization
struct Game
{
	State gameState = {};
	int score = 0;
	int wallCount = 5;

	// Aliens shooting
	float shootTimer = 0;

	bool newHighScore = false;
	
	// Entity Storage and Resources
	Resources resources;
	Player player;
	std::vector<Projectile> Projectiles;
	std::vector<Wall> Walls;
	std::vector<Alien> Aliens;
	AlienFormation alienFormation;
	Leaderboard leaderboard;
	Background background;

	void Start();
	void End();

	void Continue();
	void Launch();

	void Update();
	void Render();

	void SpawnAliens();

	bool CheckCollision(Vector2 circlePos, float circleRadius, Vector2 lineTop, Vector2 lineBottom);
};