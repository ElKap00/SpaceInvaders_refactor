#include "player.h"
#include <iostream>

Player::Player(float positionX) noexcept
	: positionX_(positionX) {}

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

	positionX_ += speed_ * direction_;

	if (positionX_ < 0.0f + radius_)
	{
		positionX_ = 0.0f + radius_;
	}
	else if (positionX_ > GetScreenWidthF() - radius_)
	{
		positionX_ = GetScreenWidthF() - radius_;
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
		static_cast<int>(positionX_ - 100.0f),
		static_cast<int>(GetScreenHeight() - height_ - 100.0f),
		WHITE);
}

