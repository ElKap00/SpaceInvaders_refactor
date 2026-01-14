#pragma once
#include "raylib_wrapper.h"

struct Wall
{
public:
	Vector2 position_ = {0.0f, 0.0f};
	static constexpr float width_ = 150.0f;
	static constexpr float height_ = 60.0f;
	bool isActive_ = true;
	int health_ = 50;

	TextureResource barrierTexture_{ "./Assets/Barrier.png" };

	static constexpr float textureWidth_ = 200.0f;
	static constexpr float textureHeight_ = 200.0f;
	static constexpr float textureOffsetX_ = textureWidth_ / 2.0f;  // 100.0f
	static constexpr float textureOffsetY_ = textureHeight_ / 2.0f; // 100.0f
	static constexpr float collisionBoxOffsetX_ = width_ / 2.0f;   // 75.0f
	static constexpr float collisionBoxOffsetY_ = 0.0f;  // No Y offset for collision box
	static constexpr float healthTextOffsetX_ = 21.0f;
	static constexpr float healthTextOffsetY_ = 10.0f;
	static constexpr int healthTextSize_ = 40;

	Wall() = default;

	explicit Wall(Vector2 position)
		: position_(position)
	{}

	Rectangle getCollisionBox() const noexcept
	{
		return { position_.x - collisionBoxOffsetX_, position_.y - collisionBoxOffsetY_, width_, height_ };
	}

	void update() noexcept
	{
		if (health_ < 1)
		{
			isActive_ = false;
		}
	}

	void render() const noexcept
	{
		DrawTexture(barrierTexture_,
			static_cast<int>(position_.x - textureOffsetX_),
			static_cast<int>(position_.y - textureOffsetY_),
			WHITE);

		DrawText(TextFormat("%i", health_), 
			gsl::narrow_cast<int>(position_.x - healthTextOffsetX_), 
			gsl::narrow_cast<int>(position_.y + healthTextOffsetY_), 
			healthTextSize_, 
			RED);
	}
};