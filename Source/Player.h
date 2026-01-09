#pragma once
#include "raylib_wrapper.h"
#include "entity_type.h"

struct Player
{
	Vector2 position_ = { GetScreenWidthF() / 2.0f, GetScreenHeightF() - 130.0f };
	float speed_ = 7.0f;
	float height_ = 70.0f;
	float radius_ = 50.0f;
	int lives_ = 3;
	int direction_ = 0;
	int activeTexture_ = 0;
	float timer_ = 0;

	EntityType type_ = EntityType::PLAYER;

	Player() noexcept = default;
	~Player() noexcept = default;

	int getLives() noexcept;
	float getPositionX() noexcept { return position_.x; }

	void render(Texture2D texture) noexcept;
	void update() noexcept;
};
