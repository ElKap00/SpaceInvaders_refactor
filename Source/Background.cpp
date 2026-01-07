#include "raylib_wrapper.h"
#include "Background.h"

Background::Background() noexcept
{
	initialize(600);
}

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

void Background::initialize(int starAmount)
{
	for (int i = 0; i < starAmount; i++)
	{
		const Vector2 initPos = { 
			static_cast<float>(GetRandomValue(-150, GetScreenWidth() + 150)),
			static_cast<float>(GetRandomValue(0, GetScreenHeight()))
		};
		const float size = static_cast<float>(GetRandomValue(1, 4)) / 2.0f;
		
		stars_.emplace_back(initPos, SKYBLUE, size);
	}
}

void Background::update(float offset) noexcept
{
	for (auto& star : stars_)
	{
		star.update(offset);
	}
}

void Background::updateWithPlayerPosition(float playerX, float playerHeight) noexcept
{
	const Vector2 playerPos = { playerX, playerHeight };
	const Vector2 cornerPos = { 0, playerHeight };
	
	const float dx = playerPos.x - cornerPos.x;
	const float dy = playerPos.y - cornerPos.y;
	const float distance = sqrtf(dx * dx + dy * dy);
	
	const float offset = distance * -1.0f / 15.0f;
	update(offset);
}

void Background::render() noexcept
{
	for (auto& star : stars_)
	{
		star.render();
	}
}