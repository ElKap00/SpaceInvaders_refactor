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