#pragma once

#include "LoggedObject.h"

#include <SFML\Graphics.hpp>

#include <vector>

#ifdef PROTOENG_EXPORTS
#define PROTOENG_API __declspec(dllexport) 
#else
#define PROTOENG_API __declspec(dllimport) 
#endif

//TODO implement iterator for Layer
namespace ProtoEng
{
	class Layer : public LoggedObject
	{
	public:
		PROTOENG_API virtual ~Layer();

		PROTOENG_API void addElement(sf::Drawable* element);
		PROTOENG_API sf::Drawable* getElement(int i) const;
		PROTOENG_API size_t getSize() const;
		PROTOENG_API void clearLayer();


	private:
		std::vector<sf::Drawable*> elements;
	};
}