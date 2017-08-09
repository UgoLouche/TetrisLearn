#pragma once

#include <SFML\Graphics.hpp>

#ifdef PROTOENG_EXPORTS
#define PROTOENG_API __declspec(dllexport)
#else
#define PROTOENG_API __declspec(dllimport)
#endif

namespace ProtoEng
{
	class Box : public sf::Drawable, public sf::Transformable
	{
	public:
		PROTOENG_API Box();
		PROTOENG_API Box(sf::Vector2f dimensions);


		PROTOENG_API void setBorderProperties(sf::Color color, float thickness);
		PROTOENG_API void setBorderColor(sf::Color color);
		PROTOENG_API void setBorderThickness(float thickness);
		PROTOENG_API void setWidth(float width);
		PROTOENG_API void setHeight(float height);
		PROTOENG_API void setDimensions(sf::Vector2f dimensions);

	private:
		sf::RectangleShape border;

		PROTOENG_API virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
	};
}