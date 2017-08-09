#pragma once

#include <SFML\Graphics.hpp>

#include <list>

#ifdef PROTOENG_EXPORTS
#define PROTOENG_API __declspec(dllexport) 
#else
#define PROTOENG_API __declspec(dllimport) 
#endif

namespace ProtoEng
{
	class Container : public sf::Drawable, public sf::Transformable
	{
	public:
		PROTOENG_API Container();
		PROTOENG_API ~Container();

		PROTOENG_API void registerItem(sf::Drawable* item);

	private:
		//Overrides
		PROTOENG_API virtual void draw(sf::RenderTarget & target, sf::RenderStates states) const override;
		PROTOENG_API virtual void draw_custom(sf::RenderTarget & target, sf::RenderStates combinesStates) const;

		std::list<sf::Drawable*> items;
	};
}