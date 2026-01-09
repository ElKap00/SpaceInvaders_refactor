#pragma once
#include "raylib_wrapper.h"

struct Player
{
	Vector2 position_ = { GetScreenWidthF() / 2.0f, GetScreenHeightF() - 50.0f };
	float speed_ = 7.0f;
	float height_ = 70.0f;
	float radius_ = 50.0f;
	int lives_ = 3;
	int direction_ = 0;
	int activeTexture_ = 0;
	float timer_ = 0;

	Rectangle collisionBox_ = { position_.x - 50.0f, position_.y - 50.0f, 100.0f, 100.0f };

	Player() noexcept = default;
	~Player() noexcept = default;

	int getLives() noexcept;
	float getPositionX() noexcept { return position_.x; }

	void render(Texture2D texture) noexcept;
	void update() noexcept;
};
