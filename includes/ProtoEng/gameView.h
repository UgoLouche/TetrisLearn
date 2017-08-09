#pragma once

#include <SFML\Graphics.hpp>

#ifdef PROTOENG_EXPORTS
#define PROTOENG_API __declspec(dllexport)
#else
#define PROTOENG_API __declspec(dllimport)
#endif

namespace ProtoEng
{
	class Layer;

	class GameView : public sf::View
	{
	public:
		PROTOENG_API GameView();
		PROTOENG_API GameView(Layer* layer);
		PROTOENG_API ~GameView();

		PROTOENG_API void assignLayer(Layer* layer);
		PROTOENG_API Layer* getAssignedLayer() const;
		PROTOENG_API void setAnchor(sf::Vector2f pos); //THis is the view position in game. Not the view position on the window

	private:
		Layer* linkedLayer;
	};
}