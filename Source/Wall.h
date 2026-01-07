#pragma once
#include "raylib_wrapper.h"

struct Wall
{
public:
	Vector2 position_ = {0.0f, 0.0f};
	bool isActive_ = true;
	int health_ = 50;
	int radius_ = 60;

	Wall() = default;
	Wall(Vector2 position) noexcept;

	bool isActive() const noexcept;
	void makeInactive() noexcept;

	void render(Texture2D texture) noexcept;
	void update() noexcept;
};