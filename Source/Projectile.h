#pragma once
#include "raylib_wrapper.h"

struct Projectile
{
public:
	Vector2 position_ = {0.0f, 0.0f};
	int speed_ = 15;
	bool isActive_ = true;

	Rectangle collisionBox_ = {0.0f, 0.0f, 10.0f, 30.0f}; // Small width, tall height for laser beam

	Projectile(Vector2 position) noexcept
		: position_(position)
	{
		// Initialize collision box
		collisionBox_.x = position_.x - 5.0f;
		collisionBox_.y = position_.y - 15.0f;
	}

	Projectile(Vector2 position, int speed) noexcept
		: position_(position), speed_(speed)
	{
		// Initialize collision box
		collisionBox_.x = position_.x - 5.0f;
		collisionBox_.y = position_.y - 15.0f;
	}

	void setActive(bool isActive) noexcept { isActive_ = isActive; }

	void update() noexcept
	{
		position_.y -= speed_;

		// Update collision box position
		collisionBox_.x = position_.x - 5.0f;
		collisionBox_.y = position_.y - 15.0f;

		if (position_.y < 0.0f || position_.y > GetScreenHeightF())
		{
			setActive(false);
		}
	}

	void render(Texture2D texture) noexcept
	{
		DrawTexture(texture,
			static_cast<int>(position_.x - 25.0f),
			static_cast<int>(position_.y - 25.0f),
			WHITE);
	}
};