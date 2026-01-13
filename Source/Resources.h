#pragma once
#include "raylib_wrapper.h"
#include "vector"
#include <iostream>

// TODO: move textures & loading to respective classes
//TODO remember that RAII types alwas only handle one resource each. eg. the ctor grabs ONE thing, validates it or throws. And release in dtor. 
// then you can make collections /aggregates out of those resource handles, if need be. 
struct Resources 
{
	std::vector<Texture2D> shipTextures_;
	Texture2D alienTexture_;
	Texture2D barrierTexture_;
	Texture2D laserTexture_;

	Resources()
	{
		std::cout << "Loading resources...\n";
		alienTexture_ = LoadTexture("./Assets/Alien.png");
		barrierTexture_ = LoadTexture("./Assets/Barrier.png");
		shipTextures_.push_back(LoadTexture("./Assets/Ship1.png"));
		shipTextures_.push_back(LoadTexture("./Assets/Ship2.png"));
		shipTextures_.push_back(LoadTexture("./Assets/Ship3.png"));
		laserTexture_ = LoadTexture("./Assets/Laser.png");
	}

	~Resources()
	{
		UnloadTexture(alienTexture_);
		UnloadTexture(barrierTexture_);
		UnloadTexture(laserTexture_);

		for (auto& texture : shipTextures_)
		{
			UnloadTexture(texture);
		}
	}
};