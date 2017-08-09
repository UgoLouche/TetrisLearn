#pragma once

#include "SFML\Graphics.hpp"

class IGraphicObject : public sf::Drawable, public sf::Transformable
{
public:
	virtual ~IGraphicObject();

	void notifyGraphicUpdate(sf::Time deltaTime);

private:
	virtual void notifyGraphicUpdate_custom(sf::Time deltaTime) = 0;
};