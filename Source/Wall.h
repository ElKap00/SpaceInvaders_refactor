#pragma once
#include "raylib_wrapper.h"

struct Wall
{
public:
	Vector2 position_;
	bool isActive_;
	int health_ = 50;
	int radius_ = 60;

	void render(Texture2D texture);
	void update();
};