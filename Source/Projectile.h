#pragma once
#include "raylib_wrapper.h"

struct Projectile
{
public:
	Vector2 position_ = {0.0f, 0.0f};
	int speed_ = 15;
	bool isActive_ = true;

	Rectangle collisionBox_ = {0.0f, 0.0f, 10.0f, 30.0f}; // Small width, tall height for laser beam

	Projectile(Vector2 position) noexcept;
	Projectile(Vector2 position, int speed) noexcept;

	void setActive(bool isActive) noexcept;

	void update() noexcept;
	void render(Texture2D texture) noexcept;
};