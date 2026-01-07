#include "alien.h"

Alien::Alien(Vector2 position) noexcept
{
	position_ = position;
}

void Alien::setActive(bool isActive) noexcept
{
	isActive_ = isActive;
}

void Alien::update() noexcept
{
	if (moveRight_)
	{
		position_.x += speed_;

		if (position_.x >= GetScreenWidth())
		{
			moveRight_ = false;
			position_.y += 50.0f;
		}
	}
	else
	{
		position_.x -= speed_;

		if (position_.x <= 0.0f)
		{
			moveRight_ = true;
			position_.y += 50.0f;
		}
	}
}

void Alien::render(Texture2D texture) noexcept
{
	DrawTexturePro(texture,
		{
			0.0f,
			0.0f,
			352.0f,
			352.0f,
		},
		{
			position_.x,
			position_.y,
			100.0f,
			100.0f,
		}, {50.0f , 50.0f},
		0.0f,
		WHITE);
}