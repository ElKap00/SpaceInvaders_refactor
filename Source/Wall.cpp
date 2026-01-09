#include "wall.h"

Wall::Wall(Vector2 position) noexcept
{
	position_ = position;
}

bool Wall::isActive() const noexcept
{
	return isActive_;
}

void Wall::makeInactive() noexcept
{
	isActive_ = false;
}

void Wall::update() noexcept
{
	if (health_ < 1)
	{
		makeInactive();
	}

	collisionBox_ = { position_.x - 50.0f, position_.y - 50.0f, 100.0f, 100.0f };
}

void Wall::render(Texture2D texture) noexcept
{
	DrawTexture(texture,
		static_cast<int>(position_.x - 100.0f),
		static_cast<int>(position_.y - 100.0f),
		WHITE);

	DrawText(TextFormat("%i", health_), position_.x - 21, position_.y + 10, 40, RED);
}