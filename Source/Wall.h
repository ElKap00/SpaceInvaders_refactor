#pragma once
#include "raylib_wrapper.h"

struct Wall
{
public:
	Vector2 position_ = {0.0, 0.0};
	bool isActive_ = true;
	int health_ = 50;
	int radius_ = 60;

	Wall() = default;
	Wall(Vector2 position) noexcept;

	bool isActive() const noexcept;
	void makeInactive() noexcept;

	void render(Texture2D texture);
	void update() noexcept;
};