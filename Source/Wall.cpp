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


void Wall::render(Texture2D texture)
{
	DrawTexturePro(texture,
		{
			0,
			0,
			704,
			704,
		},
		{
			position_.x,
			position_.y,
			200,
			200,
		}, { 100 , 100 },
		0,
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