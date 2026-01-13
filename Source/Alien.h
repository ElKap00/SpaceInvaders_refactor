#pragma once
#include "raylib_wrapper.h"

struct Alien
{
	Vector2 position_ = {0.0f, 0.0f};
	int speed_ = 2;
	bool isActive_ = true;
	TextureResource alienTexture_{ "./Assets/Alien.png" };

	Rectangle collisionBox_ = { position_.x - 25.0f, position_.y - 25.0f, 50.0f, 50.0f };

	explicit Alien(Vector2 position)
	{
		position_ = position;
	}

	void update() noexcept
	{
		position_.x += speed_;

		if (position_.x >= GetScreenWidthF() - 25.0f)
		{
			speed_ = -speed_;
			position_.y += 50.0f;
		}
		else if (position_.x <= 25.0f)
		{
			speed_ = -speed_;
			position_.y += 50.0f;
		}

		collisionBox_ = { position_.x - 25.0f, position_.y - 25.0f, 50.0f, 50.0f };
	}

	void render() const noexcept
	{
		DrawTexture(alienTexture_,
			static_cast<int>(position_.x - 50.0f),
			static_cast<int>(position_.y - 50.0f),
			WHITE);
	}
};	