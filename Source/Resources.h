#pragma once
#include "raylib_wrapper.h"
#include "vector"

struct Resources 
{
	std::vector<Texture2D> shipTextures_;
	Texture2D alienTexture_;
	Texture2D barrierTexture_;
	Texture2D laserTexture_;

	Resources();
	~Resources();
	Resources(const Resources& other);
	Resources& operator=(const Resources& other);
	Resources(Resources&& other) noexcept;
	Resources& operator=(Resources&& other) noexcept;
};