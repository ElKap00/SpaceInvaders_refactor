#include "player.h"
#include <iostream>

void Player::update() noexcept
{
	updateMovement();
	updateAnimation();

	collisionBox_ = { position_.x - 50.0f, position_.y - 50.0f, 100.0f, 100.0f };
}

void Player::updateMovement() noexcept
{
	if (IsKeyDown(KEY_LEFT))
	{
		position_.x -= speed_;
	}
	if (IsKeyDown(KEY_RIGHT))
	{
		position_.x += speed_;
	}

	if (position_.x < 0.0f + radius_)
	{
		position_.x = 0.0f + radius_;
	}
	else if (position_.x > GetScreenWidthF() - radius_)
	{
		position_.x = GetScreenWidthF() - radius_;
	}
}

void Player::updateAnimation() noexcept
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

void Player::render(Texture2D texture) noexcept
{
	DrawTexture(texture, 
		static_cast<int>(position_.x - 50.0f),
		static_cast<int>(position_.y - 50.0f),
		WHITE);
}

