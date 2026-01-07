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

	void update(float starOffset) noexcept;
	void render() noexcept;
};

struct Background
{
	std::vector<Star> stars_;

	Background() noexcept;

	void initialize(int starAmount);
	void updateWithPlayerPosition(float playerX, float playerHeight) noexcept;
	void render() noexcept;

private:
	void update(float offset) noexcept;
};