#pragma once
#include "raylib_wrapper.h"

struct Projectile
{
	Vector2 position_ = {0.0f, 0.0f};
	//TODO: the position and collisionbox encode the sam einfromation. Try to get rid of one of them. 
	//either position + width / height
	//or just rectangle. 
	// you can always have a Rectangle getCollisionBox() const noexcept { return {x, y, width, height}; } 
	int speed_ = 15;
	bool isActive_ = true;

	Rectangle collisionBox_ = {0.0f, 0.0f, 10.0f, 30.0f};

	Projectile(Vector2 position) noexcept
		: position_(position)
	{
		collisionBox_.x = position_.x - 5.0f;
		collisionBox_.y = position_.y - 15.0f;
	}

	Projectile(Vector2 position, int speed) noexcept
		: position_(position), speed_(speed)
	{
		collisionBox_.x = position_.x - 5.0f;
		collisionBox_.y = position_.y - 15.0f;
	}

	// TODO: remove setter?
	void setActive(bool isActive) noexcept { isActive_ = isActive; }

	void update() noexcept
	{
		position_.y -= speed_;

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