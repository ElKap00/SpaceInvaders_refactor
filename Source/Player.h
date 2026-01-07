#pragma once
#include "raylib_wrapper.h"
#include "entity_type.h"

struct Player
{
	// TODO: make variables private and add getters/setters as needed
public:
	// TODO: change from float to int if no fractional values are needed
	float positionX_ = (float)GetScreenWidth()/2;
	float speed_ = 7;
	float height_ = 70.0f;
	float radius_ = 50;
	int lives_ = 3;
	int direction_ = 0;
	int activeTexture_ = 0;
	float timer_ = 0;

	EntityType type_ = EntityType::PLAYER;

	Player() noexcept = default;
	Player(float positionX) noexcept;

	int getLives() noexcept;

	void initialize();
	void render(Texture2D texture) noexcept;
	void update() noexcept;
};
