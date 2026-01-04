#pragma once
#include "raylib_wrapper.h"
#include "entity_type.h"

struct Alien
{
public:
	EntityType type_ = EntityType::ENEMY;
	Color color_ = WHITE;
	Vector2 position_ = { 0, 0 };
	float radius_ = 30;
	bool isActive_ = true;
	bool moveRight_ = true;
	int speed_ = 2;

	Alien(Vector2 position) noexcept;

	void setActive(bool isActive) noexcept;

	void update();
	void render(Texture2D texture);
};