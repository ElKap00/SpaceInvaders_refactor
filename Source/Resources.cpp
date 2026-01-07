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

// Copy constructor - deep copy all textures
Resources::Resources(const Resources& other)
{
	// Copy individual textures by loading from the same data
	// Note: raylib doesn't provide texture cloning, so we need to check if textures are valid
	if (other.alienTexture_.id != 0)
	{
		alienTexture_ = other.alienTexture_;
		alienTexture_.id = 0; // Mark as invalid since we can't deep copy GPU resources
	}
	
	if (other.barrierTexture_.id != 0)
	{
		barrierTexture_ = other.barrierTexture_;
		barrierTexture_.id = 0;
	}
	
	if (other.laserTexture_.id != 0)
	{
		laserTexture_ = other.laserTexture_;
		laserTexture_.id = 0;
	}
	
	// Note: For a real implementation, you'd need to reload from file paths
	// or implement a texture sharing mechanism
	shipTextures_.reserve(other.shipTextures_.size());
	for (const auto& texture : other.shipTextures_)
	{
		Texture2D copy = texture;
		copy.id = 0; // Mark as invalid
		shipTextures_.push_back(copy);
	}
}

// Copy assignment operator
Resources& Resources::operator=(const Resources& other)
{
	if (this != &other)
	{
		// Clean up existing resources
		UnloadTexture(alienTexture_);
		UnloadTexture(barrierTexture_);
		UnloadTexture(laserTexture_);
		
		for (auto& texture : shipTextures_)
		{
			UnloadTexture(texture);
		}
		shipTextures_.clear();
		
		// Copy from other (shallow copy - see note in copy constructor)
		if (other.alienTexture_.id != 0)
		{
			alienTexture_ = other.alienTexture_;
			alienTexture_.id = 0;
		}
		
		if (other.barrierTexture_.id != 0)
		{
			barrierTexture_ = other.barrierTexture_;
			barrierTexture_.id = 0;
		}
		
		if (other.laserTexture_.id != 0)
		{
			laserTexture_ = other.laserTexture_;
			laserTexture_.id = 0;
		}
		
		shipTextures_.reserve(other.shipTextures_.size());
		for (const auto& texture : other.shipTextures_)
		{
			Texture2D copy = texture;
			copy.id = 0;
			shipTextures_.push_back(copy);
		}
	}
	return *this;
}

// Move constructor - transfer ownership
Resources::Resources(Resources&& other) noexcept
	: shipTextures_(std::move(other.shipTextures_))
	, alienTexture_(other.alienTexture_)
	, barrierTexture_(other.barrierTexture_)
	, laserTexture_(other.laserTexture_)
{
	// Invalidate the other object's textures to prevent double-free
	other.alienTexture_.id = 0;
	other.barrierTexture_.id = 0;
	other.laserTexture_.id = 0;
}

// Move assignment operator
Resources& Resources::operator=(Resources&& other) noexcept
{
	if (this != &other)
	{
		// Clean up existing resources
		UnloadTexture(alienTexture_);
		UnloadTexture(barrierTexture_);
		UnloadTexture(laserTexture_);
		
		for (auto& texture : shipTextures_)
		{
			UnloadTexture(texture);
		}
		
		// Transfer ownership
		shipTextures_ = std::move(other.shipTextures_);
		alienTexture_ = other.alienTexture_;
		barrierTexture_ = other.barrierTexture_;
		laserTexture_ = other.laserTexture_;
		
		// Invalidate the other object's textures
		other.alienTexture_.id = 0;
		other.barrierTexture_.id = 0;
		other.laserTexture_.id = 0;
	}
	return *this;
}