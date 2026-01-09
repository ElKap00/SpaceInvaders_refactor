#include "player.h"
#include <iostream>

int Player::getLives() noexcept
{
	return lives_;
}

void Player::update() noexcept
{
	//Movement
	direction_ = 0;
	if (IsKeyDown(KEY_LEFT))
	{
		direction_--;
	}
	if (IsKeyDown(KEY_RIGHT))
	{
		direction_++;
	}

	position_.x += speed_ * direction_;

	if (position_.x < 0.0f + radius_)
	{
		position_.x = 0.0f + radius_;
	}
	else if (position_.x > GetScreenWidthF() - radius_)
	{
		position_.x = GetScreenWidthF() - radius_;
	}

	//Determine frame for animation
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
		static_cast<int>(position_.x - 100.0f),
		static_cast<int>(GetScreenHeight() - height_ - 100.0f),
		WHITE);
}

