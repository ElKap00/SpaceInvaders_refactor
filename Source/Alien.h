#pragma once
#include "raylib_wrapper.h"

struct Alien
{
	Vector2 position_ = {0.0f, 0.0f};
	int speed_ = 2;
	bool isActive_ = true;

	Rectangle collisionBox_ = { position_.x - 25.0f, position_.y - 25.0f, 50.0f, 50.0f };

	Alien(Vector2 position) noexcept;

	void update() noexcept;
	void render(Texture2D texture) noexcept;
};