#include "projectile.h"

Projectile::Projectile(Vector2 position) noexcept
	: position_(position)
{
	// Initialize collision box
	collisionBox_.x = position_.x - 5.0f;
	collisionBox_.y = position_.y - 15.0f;
	collisionBox_.width = 10.0f;
	collisionBox_.height = 30.0f;
}

Projectile::Projectile(Vector2 position, int speed) noexcept
	: position_(position), speed_(speed)
{
	// Initialize collision box
	collisionBox_.x = position_.x - 5.0f;
	collisionBox_.y = position_.y - 15.0f;
	collisionBox_.width = 10.0f;
	collisionBox_.height = 30.0f;
}

void Projectile::setActive(bool isActive) noexcept { isActive_ = isActive; }

void Projectile::update() noexcept
{
	position_.y -= speed_;

	// Update collision box position
	collisionBox_.x = position_.x - 5.0f;
	collisionBox_.y = position_.y - 15.0f;

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
