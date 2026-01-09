#include "projectile.h"

Projectile::Projectile(Vector2 position, EntityType type) noexcept
	: position_(position), type_(type)
{
	// INITIAL LINE POSITIONS
	lineStart_.y = position_.y - 15.0f;
	lineEnd_.y = position_.y + 15.0f;
	lineStart_.x = position_.x;
	lineEnd_.x = position_.x;
}

Projectile::Projectile(Vector2 position, int speed, EntityType type) noexcept
	: position_(position), speed_(speed), type_(type)
{
	// INITIAL LINE POSITIONS
	lineStart_.y = position_.y - 15.0f;
	lineEnd_.y = position_.y + 15.0f;
	lineStart_.x = position_.x;
	lineEnd_.x = position_.x;
}

void Projectile::setActive(bool isActive) noexcept { isActive_ = isActive; }

void Projectile::update() noexcept
{
	position_.y -= speed_;

	// UPDATE LINE POSITION
	lineStart_.y = position_.y - 15.0f;
	lineEnd_.y = position_.y + 15.0f;

	lineStart_.x = position_.x;
	lineEnd_.x = position_.x;

	if (position_.y < 0.0f || position_.y > 1500.0f)
	{
		setActive(false);
	}
}

void Projectile::render(Texture2D texture) noexcept
{
	DrawTexture(texture,
		static_cast<int>(position_.x - 25.0f),
		static_cast<int>(position_.y - 25.0f),
		WHITE);
}
