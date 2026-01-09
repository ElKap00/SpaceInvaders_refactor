#pragma once
#include "raylib_wrapper.h"

struct Wall
{
public:
	Vector2 position_ = {0.0f, 0.0f};
	bool isActive_ = true;
	int health_ = 50;

	Rectangle collisionBox_ = { position_.x - 75.0f, position_.y, 150.0f, 60.0f };

	Wall() = default;

	Wall(Vector2 position) noexcept
	{
		position_ = position;
	}

	void update() noexcept
	{
		if (health_ < 1)
		{
			isActive_ = false;
		}

		collisionBox_ = { position_.x - 75.0f, position_.y, 150.0f, 60.0f };
	}

	void render(Texture2D texture) noexcept
	{
		DrawTexture(texture,
			static_cast<int>(position_.x - 100.0f),
			static_cast<int>(position_.y - 100.0f),
			WHITE);

		DrawText(TextFormat("%i", health_), position_.x - 21, position_.y + 10, 40, RED);
	}
};