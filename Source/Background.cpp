#include "raylib_wrapper.h"
#include "Background.h"


// TODO: write constructor for Star?
//BACKGROUND
void Star::update(float starOffset)
{
	position_.x = initPosition_.x + starOffset;
	position_.y = initPosition_.y;

}

void Star::render()
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

// TODO: use ranged for-loops
void Background::update(float offset)
{
	for (int i = 0; i < stars_.size(); i++)
	{
		stars_[i].update(offset);
	}

}

void Background::render()
{
	for (int i = 0; i < stars_.size(); i++)
	{
		stars_[i].render();
	}
}