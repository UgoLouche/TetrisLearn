#pragma once

#include "ProtoEng\TimedEvent.h"

enum class TimerFlag
{
	Stop,
	Triggered
};

class TetrisTimer : public ProtoEng::TimedEvent
{
public:
	TetrisTimer(size_t triggerTime);

	void stop();

	TimerFlag getFlag();

private:
	virtual void trigger_custom() override;

	TimerFlag triggerFlag;
};