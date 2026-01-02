#include "resources.h"
#include <iostream>

void Resources::load()
{
	std::cout << "Yay"; // yes
	alienTexture_ = LoadTexture("./Assets/Alien.png");
	barrierTexture_ = LoadTexture("./Assets/Barrier.png");
	shipTextures_.push_back(LoadTexture("./Assets/Ship1.png"));
	shipTextures_.push_back(LoadTexture("./Assets/Ship2.png"));
	shipTextures_.push_back(LoadTexture("./Assets/Ship3.png"));
	laserTexture_ = LoadTexture("./Assets/Laser.png");
}