#include "player.h"
#include <iostream>

Player::Player(float positionX)
	: positionX_(positionX) {}

int Player::getLives()
{
	return lives_;
}

void Player::initialize()
{
	// TODO: fix C-style casts
	float window_width = (float)GetScreenWidth();
	positionX_ = window_width / 2;
	std::cout << "Find Player -X:" << GetScreenWidth() / 2 << "Find Player -Y" << GetScreenHeight() - height_ << std::endl;
}

void Player::update()
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

	if (positionX_ < 0 + radius_)
	{
		positionX_ = 0 + radius_;
	}
	else if (positionX_ > GetScreenWidth() - radius_)
	{
		positionX_ = GetScreenWidth() - radius_;
	}

	//Determine frame for animation
	timer_ += GetFrameTime();

	if (timer_ > 0.4 && activeTexture_ == 2)
	{
		activeTexture_ = 0;
		timer_ = 0;
	}
	else if (timer_ > 0.4)
	{
		activeTexture_++;
		timer_ = 0;
	}
}

void Player::render(Texture2D texture)
{
	// TODO: make variable const int
	float window_height = GetScreenHeight();

	DrawTexturePro(texture,
		{
			0,
			0,
			352,
			352,
		},
		{
			positionX_, window_height - height_,
			100,
			100,
		}, { 50, 50 },
		0,
		WHITE);
}

