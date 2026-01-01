#pragma once
#include "raylib_wrapper.h"
#include "EntityType.h"

// TODO: write constructor for Player
struct Player
{
	// TODO: make variables private and add getters/setters as needed
public:
	// TODO: change from float to int if no fractional values are needed
	float x_pos = 0;
	float speed = 7;
	float player_base_height = 70.0f;
	float radius = 50;
	int lives = 3;
	int direction = 0;
	int activeTexture = 0;
	float timer = 0;

	EntityType type = EntityType::PLAYER;

	void Initialize();
	void Render(Texture2D texture);
	void Update();

};
