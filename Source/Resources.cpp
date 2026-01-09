#include "resources.h"
#include <iostream>

Resources::Resources()
{
	std::cout << "Loading resources...\n";
	alienTexture_ = LoadTexture("./Assets/Alien.png");
	barrierTexture_ = LoadTexture("./Assets/Barrier.png");
	shipTextures_.push_back(LoadTexture("./Assets/Ship1.png"));
	shipTextures_.push_back(LoadTexture("./Assets/Ship2.png"));
	shipTextures_.push_back(LoadTexture("./Assets/Ship3.png"));
	laserTexture_ = LoadTexture("./Assets/Laser.png");
}

Resources::~Resources()
{
	UnloadTexture(alienTexture_);
	UnloadTexture(barrierTexture_);
	UnloadTexture(laserTexture_);
	
	for (auto& texture : shipTextures_)
	{
		UnloadTexture(texture);
	}
}