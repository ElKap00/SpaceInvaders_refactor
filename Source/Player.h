#pragma once
#include "raylib_wrapper.h"
#include <iostream>

struct Player
{
	Vector2 position_ = { GetScreenWidthF() / 2.0f, GetScreenHeightF() - 100.0f };
	static constexpr float width_ = 100.0f;
	static constexpr float height_ = 100.0f;
	float speed_ = 7.0f;
	int lives_ = 3;

	std::vector<TextureResource> shipTextures_;
	float animationTimer_ = 0.0f;
	int activeTexture_ = 0;
	static constexpr float textureWidth_ = 100.0f;
	static constexpr float textureHeight_ = 100.0f;
	static constexpr float textureOffsetX_ = textureWidth_ / 2.0f;  // 50.0f
	static constexpr float textureOffsetY_ = textureHeight_ / 2.0f; // 50.0f
	static constexpr float collisionBoxOffsetX_ = width_ / 2.0f;   // 50.0f
	static constexpr float collisionBoxOffsetY_ = height_ / 2.0f;  // 50.0f

	Player()
	{
		shipTextures_.emplace_back("./Assets/Ship1.png");
		shipTextures_.emplace_back("./Assets/Ship2.png");
		shipTextures_.emplace_back("./Assets/Ship3.png");
	}

	Rectangle getCollisionBox() const noexcept
	{
		return { position_.x - collisionBoxOffsetX_, position_.y - collisionBoxOffsetY_, width_, height_ };
	}

	float getPositionX() const noexcept { return position_.x; }

	bool isAlive() const noexcept { return lives_ > 0; }

	void update() noexcept
	{
		updateMovement();
		updateAnimation();
	}

	void updateMovement() noexcept
	{
		if (IsKeyDown(KEY_LEFT))
		{
			position_.x -= speed_;
		}
		if (IsKeyDown(KEY_RIGHT))
		{
			position_.x += speed_;
		}

		// Keep player within screen bounds
		if (position_.x < 0.0f + textureOffsetX_)
		{
			position_.x = 0.0f + textureOffsetX_;
		}
		else if (position_.x > GetScreenWidthF() - textureOffsetX_)
		{
			position_.x = GetScreenWidthF() - textureOffsetX_;
		}
	}

	void updateAnimation() noexcept
	{
		animationTimer_ += GetFrameTime();

		if (animationTimer_ > 0.4f && activeTexture_ == 2)
		{
			activeTexture_ = 0;
			animationTimer_ = 0.0f;
		}
		else if (animationTimer_ > 0.4f)
		{
			activeTexture_++;
			animationTimer_ = 0.0f;
		}
	}

	void render() const noexcept
	{
		DrawTexture(shipTextures_[activeTexture_],
			static_cast<int>(position_.x - textureOffsetX_),
			static_cast<int>(position_.y - textureOffsetY_),
			WHITE);
	}
};