#pragma once
#include "raylib_wrapper.h"

struct Alien
{
	Vector2 position_ = {0.0f, 0.0f};
	static constexpr float width_ = 50.0f;
	static constexpr float height_ = 50.0f;
	int speed_ = 2;
	bool isActive_ = true;
	
	TextureResource alienTexture_{ "./Assets/Alien.png" };
	static constexpr float textureWidth_ = 100.0f;
	static constexpr float textureHeight_ = 100.0f;
	static constexpr float textureOffsetX_ = textureWidth_ / 2.0f;  // 50.0f
	static constexpr float textureOffsetY_ = textureHeight_ / 2.0f; // 50.0f
	static constexpr float collisionBoxOffsetX_ = width_ / 2.0f;   // 25.0f
	static constexpr float collisionBoxOffsetY_ = height_ / 2.0f;  // 25.0f
	static constexpr float moveDownDistance_ = 50.0f;

	explicit Alien(Vector2 position)
		: position_(position)
	{}

	Rectangle getCollisionBox() const noexcept
	{
		return { position_.x - collisionBoxOffsetX_, position_.y - collisionBoxOffsetY_, width_, height_ };
	}

	void update() noexcept
	{
		position_.x += speed_;

		if (position_.x >= GetScreenWidthF() - collisionBoxOffsetX_)
		{
			speed_ = -speed_;
			position_.y += moveDownDistance_;
		}
		else if (position_.x <= collisionBoxOffsetX_)
		{
			speed_ = -speed_;
			position_.y += moveDownDistance_;
		}
	}

	void render() const noexcept
	{
		DrawTexture(alienTexture_,
			static_cast<int>(position_.x - textureOffsetX_),
			static_cast<int>(position_.y - textureOffsetY_),
			WHITE);
	}
};