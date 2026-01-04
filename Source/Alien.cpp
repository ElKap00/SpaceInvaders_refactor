#include "alien.h"

Alien::Alien(Vector2 position) noexcept
{
	position_ = position;
}

void Alien::setActive(bool isActive) noexcept
{
	isActive_ = isActive;
}

void Alien::update()
{
	if (moveRight_)
	{
		position_.x += speed_;

		if (position_.x >= GetScreenWidth())
		{
			moveRight_ = false;
			position_.y += 50;
		}
	}
	else
	{
		position_.x -= speed_;

		if (position_.x <= 0)
		{
			moveRight_ = true;
			position_.y += 50;
		}
	}
}

void Alien::render(Texture2D texture)
{
	DrawTexturePro(texture,
		{
			0,
			0,
			352,
			352,
		},
		{
			position_.x,
			position_.y,
			100,
			100,
		}, { 50 , 50 },
		0,
		WHITE);
}