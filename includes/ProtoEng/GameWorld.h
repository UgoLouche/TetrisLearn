#pragma once

#include "LoggedObject.h"

#include <SFML\Graphics.hpp>

#include <vector>

#ifdef PROTOENG_EXPORTS
#define PROTOENG_API __declspec(dllexport) 
#else
#define PROTOENG_API __declspec(dllimport) 
#endif

namespace ProtoEng
{
	class Layer;

	class GameWorld : public LoggedObject
	{
	public:
		PROTOENG_API GameWorld();
		PROTOENG_API virtual ~GameWorld();

		PROTOENG_API Layer* getLayer(int i);
		PROTOENG_API Layer* getLayer(int i) const; //return NULL is element i cannot be reached

		PROTOENG_API void registerElement(int layerLevel, sf::Drawable* element);

	private:

		std::vector<Layer*> layers;
	};
}