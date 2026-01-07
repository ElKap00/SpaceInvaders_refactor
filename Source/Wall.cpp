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


void Wall::render(Texture2D texture) noexcept
{
	DrawTexturePro(texture,
		{
			0.0f,
			0.0f,
			704.0f,
			704.0f,
		},
		{
			position_.x,
			position_.y,
			200.0f,
			200.0f,
		}, {100.0f , 100.0f},
		0.0f,
		WHITE);


	DrawText(TextFormat("%i", health_), position_.x - 21, position_.y + 10, 40, RED);

}

void Wall::update() noexcept
{
	if (health_ < 1)
	{
		makeInactive();
	}
}