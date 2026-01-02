#pragma once
#include "raylib_wrapper.h"
#include "entity_type.h"

struct Projectile
{
public:
	// INITIALIZE PROJECTILE WHILE DEFINING IF ITS PLAYER OR ENEMY 
	Vector2 position_ = { 0,0 };
	int speed_ = 15;
	bool active_ = true;
	EntityType type_ = {};

	// LINE WILL UPDATE WITH POSITION FOR CALCULATIONS
	Vector2 lineStart_ = { 0, 0 };
	Vector2 lineEnd_ = { 0, 0 };

	void update();
	void render(Texture2D texture);
};