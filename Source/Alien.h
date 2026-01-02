#pragma once
#include "raylib_wrapper.h"
#include "EntityType.h"

struct Alien
{
public:
	EntityType type = EntityType::ENEMY;
	Color color = WHITE;
	Vector2 position = { 0, 0 };
	float radius = 30;
	bool active = true;
	bool moveRight = true;
	int speed = 2;

	void Update();
	void Render(Texture2D texture);
};