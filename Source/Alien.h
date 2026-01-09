#pragma once
#include "raylib_wrapper.h"

struct Alien
{
	Color color_ = WHITE;
	Vector2 position_ = {0.0f, 0.0f};
	float radius_ = 30.0f;
	bool isActive_ = true;
	int speed_ = 2;

	Rectangle collisionBox_ = { position_.x - 25.0f, position_.y - 25.0f, 50.0f, 50.0f };

	Alien(Vector2 position) noexcept;

	void update() noexcept;
	void render(Texture2D texture) noexcept;
};