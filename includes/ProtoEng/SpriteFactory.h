#pragma once

#include "LoggedObject.h"

#include "SFML\Graphics.hpp"

#include <vector>

#ifdef PROTOENG_EXPORTS
#define PROTOENG_API __declspec(dllexport) 
#else
#define PROTOENG_API __declspec(dllimport) 
#endif

namespace ProtoEng
{
	class SpriteFactory : public LoggedObject
	{
	public:
		PROTOENG_API SpriteFactory();
		PROTOENG_API virtual ~SpriteFactory();

		//Load a Texture into memory and return a unique identifier to it
		PROTOENG_API size_t loadTexture(std::string path);
		//Get a loaded texture
		PROTOENG_API const sf::Texture& getTexture(size_t id) const;

		//Get a sprite with the specified texture
		PROTOENG_API sf::Sprite getSprite(size_t id) const;

	private:
		PROTOENG_API virtual std::string custom_log_header() const;

		std::vector<sf::Texture> textures;
	};
}