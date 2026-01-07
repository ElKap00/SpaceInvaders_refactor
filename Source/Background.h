#pragma once
#include "raylib_wrapper.h"
#include <vector>

struct Star
{
	Vector2 initPosition_ = { 0, 0 };
	Vector2 position_ = { 0, 0 };
	Color color_ = GRAY;
	float size_ = 0;

	void update(float starOffset) noexcept;
	void render() noexcept;
};

struct Background
{
	std::vector<Star> stars_;
	Vector2 playerPos_;
	Vector2 cornerPos_;
	float offset_;

	Background() noexcept;

	void initialize(int starAmount);
	void update(float offset) noexcept;
	void render() noexcept;
};