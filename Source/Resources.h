#pragma once
#include "raylib_wrapper.h"
#include "vector"

// TODO: implement rule of zero/five for resource management?
struct Resources 
{
	void Load();
	//void Unload();

	std::vector<Texture2D> shipTextures;
	Texture2D alienTexture;
	Texture2D barrierTexture;
	Texture2D laserTexture;

};