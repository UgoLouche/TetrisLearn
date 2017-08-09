#pragma once

#include "LoggedObject.h"

#include <SFML\Graphics.hpp>

#ifdef PROTOENG_EXPORTS
#define PROTOENG_API __declspec(dllexport) 
#else
#define PROTOENG_API __declspec(dllimport) 
#endif

namespace ProtoEng
{
	class EventHandler : public LoggedObject
	{
	public:
		PROTOENG_API virtual ~EventHandler();

		PROTOENG_API void newEvent(const sf::Event& ev);

	private:
		PROTOENG_API virtual void handleEvent(const sf::Event& ev) = 0;
	};
}