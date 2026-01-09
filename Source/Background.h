#pragma once
#include "raylib_wrapper.h"
#include <vector>

struct Star
{
	Vector2 initPosition_ = {0.0f, 0.0f};
	Vector2 position_ = {0.0f, 0.0f};
	Color color_ = GRAY;
	float size_ = 0.0f;

	Star(Vector2 initPos, Color col, float sz) noexcept
		: initPosition_(initPos), position_(initPos), color_(col), size_(sz) {}

	void update(float starOffset) noexcept
	{
		position_.x = initPosition_.x + starOffset;
		position_.y = initPosition_.y;

	}

	void render() noexcept
	{
		DrawCircle(static_cast<int>(position_.x), static_cast<int>(position_.y), size_, color_);
	}
};

struct Background
{
	std::vector<Star> stars_;

	Background() { initialize(600); }
	~Background() = default;

	void initialize(int starAmount)
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

	void update(float offset) noexcept
	{
		for (auto& star : stars_)
		{
			star.update(offset);
		}
	}

	void updateWithPlayerPosition(float playerX) noexcept
	{
		update(-playerX / 15.0f);
	}

	void render() noexcept
	{
		for (auto& star : stars_)
		{
			star.render();
		}
	}
};