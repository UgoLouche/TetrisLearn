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
	class TimedEvent;

	class GameplayManager : public LoggedObject
	{
	public:
		PROTOENG_API GameplayManager();
		PROTOENG_API virtual ~GameplayManager();

		//Frame DeltaTime handling
		PROTOENG_API void forceClockReset(); //Should never be called outside of ProtoEng
		PROTOENG_API sf::Time deltaTime() const;

		//Timed Event
		PROTOENG_API void registerTimedEvent(TimedEvent* ev);
		PROTOENG_API void clearTimedEvent();

		//4-Steps execution Scheme:
		// 1- Pre processing, and cleanup before actual gameplay stuff
		// 2- Event Handling (one call / event) (custom called FIRST)
		// 3- Gameplay updates (timed event first)
		// 4- Post-Rendering  updates 
		PROTOENG_API void preUpdate();
		PROTOENG_API void newEvent(const sf::Event& ev);
		PROTOENG_API void gameplayUpdate();
		PROTOENG_API void postUpdate();

	private:
		PROTOENG_API void triggerTimedEvents();

		PROTOENG_API virtual void preUpdate_custom();
		PROTOENG_API virtual void newEvent_custom(const sf::Event& ev);
		PROTOENG_API virtual void gameplayUpdate_custom();
		PROTOENG_API virtual void postUpdate_custom();

		sf::Clock internal_clock;
		sf::Time frameLenght;

		std::vector<TimedEvent*> t_events;
	};
}