#include "alien.h"

Alien::Alien(Vector2 position) noexcept
{
	position_ = position;
}

//TODO: _isActive is a public member, so don't bother with accessors. 
void Alien::setActive(bool isActive) noexcept
{
	isActive_ = isActive;
}

void Alien::update() noexcept 
{
	//TODO: both branches are identical. Refactor.
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
	//scale the image on the harddrive instead
	// and then use DrawTexture() instead of DrawTexturePro()
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