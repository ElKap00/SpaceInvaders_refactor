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
	//Texture2D alienTexture_;
	//Texture2D barrierTexture_;
	//Texture2D laserTexture_;

	Resources()
	{
		std::cout << "Loading resources...\n";
		//alienTexture_ = LoadTexture("./Assets/Alien.png");
		//barrierTexture_ = LoadTexture("./Assets/Barrier.png");
		shipTextures_.push_back(LoadTexture("./Assets/Ship1.png"));
		shipTextures_.push_back(LoadTexture("./Assets/Ship2.png"));
		shipTextures_.push_back(LoadTexture("./Assets/Ship3.png"));
		//laserTexture_ = LoadTexture("./Assets/Laser.png");
	}

	~Resources()
	{
		//UnloadTexture(alienTexture_);
		//UnloadTexture(barrierTexture_);
		//UnloadTexture(laserTexture_);

		for (auto& texture : shipTextures_)
		{
			UnloadTexture(texture);
		}
	}
};

#pragma once
#include "raylib_wrapper.h"
#include <stdexcept>
#include <string_view>

class TextureResource
{
private:
	Texture2D texture_{};

public:
	TextureResource() = default;

	explicit TextureResource(std::string_view path)
	{
		texture_ = LoadTexture(path.data());
		if (texture_.id == 0)
		{
			TraceLog(LOG_ERROR, "TextureResource: Failed to load texture: %s", path.data());
			throw std::runtime_error("Failed to load texture");
		}
	}

	~TextureResource()
	{
		if (texture_.id != 0)
		{
			UnloadTexture(texture_);
		}
	}

	// Delete copy operations (Texture is not copyable)
	TextureResource(const TextureResource&) = delete;
	TextureResource& operator=(const TextureResource&) = delete;

	// Implement move operations
	TextureResource(TextureResource&& other) noexcept
		: texture_(other.texture_)
	{
		other.texture_.id = 0;
	}

	TextureResource& operator=(TextureResource&& other) noexcept
	{
		if (this != &other)
		{
			if (texture_.id != 0)
			{
				UnloadTexture(texture_);
			}
			texture_ = other.texture_;
			other.texture_.id = 0;
		}
		return *this;
	}

	// Implicit conversion to Texture2D for raylib functions
	operator const Texture2D& () const noexcept { return texture_; }
	const Texture2D& get() const noexcept { return texture_; }
};