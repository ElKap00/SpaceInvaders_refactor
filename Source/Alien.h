#pragma once
#include "raylib_wrapper.h"
#include "entity_type.h"

struct Alien
{
	EntityType type_ = EntityType::ENEMY;
	Color color_ = WHITE;
	Vector2 position_ = {0.0f, 0.0f};
	float radius_ = 30.0f;
	bool isActive_ = true;
	bool moveRight_ = true; // TODO: consider removing. "speed" (eg. velcoity) already implies direction
	int speed_ = 2;

	Alien(Vector2 position) noexcept;

	void setActive(bool isActive) noexcept;

	void update() noexcept;
	void render(Texture2D texture) noexcept;
};