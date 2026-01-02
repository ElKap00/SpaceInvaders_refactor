#pragma once
#include "raylib_wrapper.h"
#include "vector"

// TODO: implement rule of zero/five for resource management?
struct Resources 
{
	std::vector<Texture2D> shipTextures_;
	Texture2D alienTexture_;
	Texture2D barrierTexture_;
	Texture2D laserTexture_;

	void load();
};