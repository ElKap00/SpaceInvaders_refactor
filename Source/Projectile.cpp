#include "projectile.h"

Projectile::Projectile(Vector2 position, EntityType type) noexcept
	: position_(position), type_(type)
{
	// INITIAL LINE POSITIONS
	lineStart_.y = position_.y - 15;
	lineEnd_.y = position_.y + 15;
	lineStart_.x = position_.x;
	lineEnd_.x = position_.x;
}

Projectile::Projectile(Vector2 position, int speed, EntityType type) noexcept
	: position_(position), speed_(speed), type_(type)
{
	// INITIAL LINE POSITIONS
	lineStart_.y = position_.y - 15;
	lineEnd_.y = position_.y + 15;
	lineStart_.x = position_.x;
	lineEnd_.x = position_.x;
}

void Projectile::setActive(bool isActive) noexcept { isActive_ = isActive; }

void Projectile::update() noexcept
{
	position_.y -= speed_;

	// UPDATE LINE POSITION
	lineStart_.y = position_.y - 15;
	lineEnd_.y = position_.y + 15;

	lineStart_.x = position_.x;
	lineEnd_.x = position_.x;

	if (position_.y < 0 || position_.y > 1500)
	{
		setActive(false);
	}
}

void Projectile::render(Texture2D texture) noexcept
{
	DrawTexturePro(texture,
		{
			0,
			0,
			176,
			176,
		},
		{
			position_.x,
			position_.y,
			50,
			50,
		}, { 25 , 25 },
		0,
		WHITE);
}
