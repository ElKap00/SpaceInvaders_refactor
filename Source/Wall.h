#pragma once
#include "raylib_wrapper.h"

struct Wall
{
public:
	Vector2 position_ = {0.0f, 0.0f};
	bool isActive_ = true;
	int health_ = 50;
	TextureResource barrierTexture_{ "./Assets/Barrier.png" };

	Rectangle collisionBox_ = { position_.x - 75.0f, position_.y, 150.0f, 60.0f };

	Wall() = default;

	explicit Wall(Vector2 position)
		: position_(position)
	{}

	void update() noexcept
	{
		if (health_ < 1)
		{
			isActive_ = false;
		}

		collisionBox_ = { position_.x - 75.0f, position_.y, 150.0f, 60.0f };
	}

	void render() const noexcept
	{
		DrawTexture(barrierTexture_,
			static_cast<int>(position_.x - 100.0f),
			static_cast<int>(position_.y - 100.0f),
			WHITE);

		DrawText(TextFormat("%i", health_), gsl::narrow_cast<int>(position_.x - 21.0f), 
											gsl::narrow_cast<int>(position_.y + 10.0f), 40, RED);
	}
};