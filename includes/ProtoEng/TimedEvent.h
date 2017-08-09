#pragma once

#include "LoggedObject.h"

#include "SFML\System.hpp"

#ifdef PROTOENG_EXPORTS
#define PROTOENG_API __declspec(dllexport)
#else
#define PROTOENG_API __declspec(dllimport)
#endif

namespace ProtoEng
{
	class TimedEvent : public LoggedObject
	{
	public:
		PROTOENG_API TimedEvent(sf::Time time = sf::seconds(1), bool recurrent = false);
		PROTOENG_API virtual ~TimedEvent();

		//Update remaining time and lauch trigger if needed
		PROTOENG_API bool check(sf::Time deltaTime); 

		PROTOENG_API void reArm();

		//Getter / Setter
		PROTOENG_API sf::Time getRemainingTime() const;

		PROTOENG_API void setBaseTime(sf::Time time);
		PROTOENG_API sf::Time getBaseTime() const;

		PROTOENG_API bool getIsRecurrent() const;
		PROTOENG_API void setReccurent(bool recurrent);

	private:
		PROTOENG_API void trigger(); //rearming is handled here, after trigger_custom is called
		PROTOENG_API virtual void trigger_custom() = 0;

		sf::Time baseTime; //Timer lenght
		sf::Time remainingTime; //Remaining time
		bool isRecurrent;

	};
}