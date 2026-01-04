#pragma once
#include "raylib_wrapper.h"
#include "entity_type.h"

struct Projectile
{
public:
	// INITIALIZE PROJECTILE WHILE DEFINING IF ITS PLAYER OR ENEMY 
	Vector2 position_ = { 0,0 };
	int speed_ = 15;
	bool isActive_ = true;
	EntityType type_ = {};

	// LINE WILL UPDATE WITH POSITION FOR CALCULATIONS
	Vector2 lineStart_ = { 0, 0 };
	Vector2 lineEnd_ = { 0, 0 };

	Projectile(Vector2 position, EntityType type) noexcept;
	Projectile(Vector2 position, int speed, EntityType type) noexcept;

	void update();
	void render(Texture2D texture);
};