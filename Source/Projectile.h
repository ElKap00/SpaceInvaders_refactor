#pragma once
#include "raylib_wrapper.h"

struct Projectile
{
	Vector2 position_ = {0.0f, 0.0f};
	//TODO: the position and collisionbox encode the same information. Try to get rid of one of them. 
	//either position + width / height
	//or just rectangle. 
	// you can always have a Rectangle getCollisionBox() const noexcept { return {x, y, width, height}; } 
	int speed_ = 15;
	bool isActive_ = true;
	TextureResource laserTexture_{ "./Assets/Laser.png" };

	Rectangle collisionBox_ = {0.0f, 0.0f, 10.0f, 30.0f};

	explicit Projectile(Vector2 position)
		: position_(position)
	{
		collisionBox_.x = position_.x - 5.0f;
		collisionBox_.y = position_.y - 15.0f;
	}

	explicit Projectile(Vector2 position, int speed)
		: position_(position), speed_(speed)
	{
		collisionBox_.x = position_.x - 5.0f;
		collisionBox_.y = position_.y - 15.0f;
	}

	void update() noexcept
	{
		position_.y -= speed_;

		collisionBox_.x = position_.x - 5.0f;
		collisionBox_.y = position_.y - 15.0f;

		if (position_.y < 0.0f || position_.y > GetScreenHeightF())
		{
			isActive_ = false;
		}
	}

	void render() const noexcept
	{
		DrawTexture(laserTexture_,
			static_cast<int>(position_.x - 25.0f),
			static_cast<int>(position_.y - 25.0f),
			WHITE);
	}
};