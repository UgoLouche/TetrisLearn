#include "TetrisTimer.hpp"

TetrisTimer::TetrisTimer(size_t triggerTime) :
	TimedEvent(sf::milliseconds((int)triggerTime), false), triggerFlag(TimerFlag::Stop)
{
	//By default inactive.
	check(sf::milliseconds((int)triggerTime + 1));
}

void TetrisTimer::stop()
{
	check(getBaseTime());

	triggerFlag = TimerFlag::Stop;
}

TimerFlag TetrisTimer::getFlag()
{
	return triggerFlag;
}

void TetrisTimer::trigger_custom()
{
	triggerFlag = TimerFlag::Triggered;
}
