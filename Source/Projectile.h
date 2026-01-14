#pragma once
#include "raylib_wrapper.h"

struct Projectile
{
	Vector2 position_ = {0.0f, 0.0f};
	static constexpr float width_ = 10.0f;
	static constexpr float height_ = 30.0f;
	int speed_ = 15;
	bool isActive_ = true;

	TextureResource laserTexture_{ "./Assets/Laser.png" };
	static constexpr float textureWidth_ = 50.0f;
	static constexpr float textureHeight_ = 50.0f;
	static constexpr float textureOffsetX_ = textureWidth_ / 2.0f;  // 25.0f
	static constexpr float textureOffsetY_ = textureHeight_ / 2.0f; // 25.0f
	static constexpr float collisionBoxOffsetX_ = width_ / 2.0f;   // 5.0f
	static constexpr float collisionBoxOffsetY_ = height_ / 2.0f;  // 15.0f

	explicit Projectile(Vector2 position)
		: position_(position)
	{}

	explicit Projectile(Vector2 position, int speed)
		: position_(position), speed_(speed)
	{}

	Rectangle getCollisionBox() const noexcept
	{
		return { position_.x - collisionBoxOffsetX_, position_.y - collisionBoxOffsetY_, width_, height_ };
	}

	void update() noexcept
	{
		position_.y -= speed_;

		if (position_.y < 0.0f || position_.y > GetScreenHeightF())
		{
			isActive_ = false;
		}
	}

	void render() const noexcept
	{
		DrawTexture(laserTexture_,
			static_cast<int>(position_.x - textureOffsetX_),
			static_cast<int>(position_.y - textureOffsetY_),
			WHITE);
	}
};