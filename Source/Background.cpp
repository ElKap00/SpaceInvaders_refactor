#include "raylib_wrapper.h"
#include "Background.h"

Background::Background() noexcept
{
	initialize(600);
}

// TODO: write constructor for Star?
//BACKGROUND
void Star::update(float starOffset) noexcept
{
	position_.x = initPosition_.x + starOffset;
	position_.y = initPosition_.y;

}

void Star::render() noexcept
{
	// TODO: fix C-style casts
	DrawCircle((int)position_.x, (int)position_.y, size_, color_);
}

// TODO: write constructor for Background
void Background::initialize(int starAmount)
{
	for (int i = 0; i < starAmount; i++)
	{
		// TODO: remove multi-step initialization
		Star newStar;

		newStar.initPosition_.x = GetRandomValue(-150, GetScreenWidth() + 150);
		newStar.initPosition_.y = GetRandomValue(0, GetScreenHeight());

		//random color?
		newStar.color_ = SKYBLUE;

		newStar.size_ = GetRandomValue(1, 4) / 2;

		stars_.push_back(newStar);

	}
}

void Background::update(float offset) noexcept
{
	for (auto& star : stars_)
	{
		star.update(offset);
	}

}

void Background::render() noexcept
{
	for (auto& star : stars_)
	{
		star.render();
	}
}