#include "alien.h"

Alien::Alien(Vector2 position) noexcept
{
	position_ = position;
}

void Alien::update() noexcept 
{
	position_.x += speed_;

	if (position_.x >= GetScreenWidthF() - 25.0f)
	{
		speed_ = -speed_;
		position_.y += 50.0f;
	}
	else if (position_.x <= 25.0f)
	{
		speed_ = -speed_;
		position_.y += 50.0f;
	}

	collisionBox_ = { position_.x - 25.0f, position_.y - 25.0f, 50.0f, 50.0f };
}

void Alien::render(Texture2D texture) noexcept
{
	DrawTexture(texture,
		static_cast<int>(position_.x - 50.0f),
		static_cast<int>(position_.y - 50.0f),
		WHITE);
}