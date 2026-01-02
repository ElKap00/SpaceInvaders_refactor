#pragma once
#include "raylib_wrapper.h"


struct Wall
{
public:
	Vector2 position;
	bool active;
	int health = 50;
	int radius = 60;

	void Render(Texture2D texture);
	void Update();
};