#pragma once

#include "LoggedObject.h"

#include <SFML\Graphics.hpp>

#include <vector>
#include <string>


#ifdef PROTOENG_EXPORTS
#define PROTOENG_API __declspec(dllexport)
#else
#define PROTOENG_API __declspec(dllimport)
#endif

namespace ProtoEng
{
	class GameView;
	class GameplayManager;

	class GameWindow : public sf::RenderWindow, public LoggedObject
	{
	public:
		PROTOENG_API GameWindow();
		PROTOENG_API GameWindow(sf::Vector2u size, std::string name);
		PROTOENG_API virtual ~GameWindow();

		PROTOENG_API void registerView(GameView* view);
		PROTOENG_API GameView* getView(int i) const;
		PROTOENG_API void setGame(GameplayManager* parent);

		PROTOENG_API void launch(); //Main game Loop

	private:
		std::vector<GameView*> registeredView;
		GameplayManager* parent;
	};
}