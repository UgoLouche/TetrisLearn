#include "IGraphicObject.hpp"

IGraphicObject::~IGraphicObject()
{
}

void IGraphicObject::notifyGraphicUpdate(sf::Time deltaTime)
{
	notifyGraphicUpdate_custom(deltaTime);
}