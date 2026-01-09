#pragma once
#include "raylib_wrapper.h"

struct Wall
{
public:
	Vector2 position_ = {0.0f, 0.0f};
	bool isActive_ = true;
	int health_ = 50;
	int radius_ = 60;

	Rectangle collisionBox_ = { position_.x - 50.0f, position_.y - 50.0f, 100.0f, 100.0f };

	Wall() = default;
	Wall(Vector2 position) noexcept;

	bool isActive() const noexcept;
	void makeInactive() noexcept;

	void render(Texture2D texture) noexcept;
	void update() noexcept;
};