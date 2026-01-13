#pragma once
#include "raylib_wrapper.h"
#include <iostream>

struct Player
{
	Vector2 position_ = { GetScreenWidthF() / 2.0f, GetScreenHeightF() - 100.0f };
	float speed_ = 7.0f;
	float radius_ = 50.0f;
	int lives_ = 3;
	int activeTexture_ = 0;
	float timer_ = 0;

	Rectangle collisionBox_ = { position_.x - 50.0f, position_.y - 50.0f, 100.0f, 100.0f };

	Player() noexcept = default;
	~Player() noexcept = default;

	float getPositionX() noexcept { return position_.x; }

	void update() noexcept
	{
		updateMovement();
		updateAnimation();

		collisionBox_ = { position_.x - 50.0f, position_.y - 50.0f, 100.0f, 100.0f };
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
		if (position_.x < 0.0f + radius_)
		{
			position_.x = 0.0f + radius_;
		}
		else if (position_.x > GetScreenWidthF() - radius_)
		{
			position_.x = GetScreenWidthF() - radius_;
		}
	}

	void updateAnimation() noexcept
	{
		timer_ += GetFrameTime();

		if (timer_ > 0.4f && activeTexture_ == 2)
		{
			activeTexture_ = 0;
			timer_ = 0.0f;
		}
		else if (timer_ > 0.4f)
		{
			activeTexture_++;
			timer_ = 0.0f;
		}
	}

	void render(Texture2D texture) noexcept //TODO: make const. 
	{
		DrawTexture(texture,
			static_cast<int>(position_.x - 50.0f), //TODO: give this magic value a name, and make it static constexpr whatever. 
			static_cast<int>(position_.y - 50.0f),
			WHITE);
	}
};