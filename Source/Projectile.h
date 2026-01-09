#pragma once
#include "raylib_wrapper.h"

struct Projectile
{
public:
	Vector2 position_ = {0.0f, 0.0f};
	int speed_ = 15;
	bool isActive_ = true;

	Vector2 lineStart_ = {0.0f, 0.0f};
	Vector2 lineEnd_ = {0.0f, 0.0f};

	Projectile(Vector2 position) noexcept;
	Projectile(Vector2 position, int speed) noexcept;

	void setActive(bool isActive) noexcept;

	void update() noexcept;
	void render(Texture2D texture) noexcept;
};