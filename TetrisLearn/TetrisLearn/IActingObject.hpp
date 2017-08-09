#pragma once

#include "ProtoEng\LoggedObject.h"

class IActingObject : public ProtoEng::LoggedObject
{
public:
	IActingObject();
	virtual ~IActingObject();

	void Act();

	void deleteLater(); //Tag for future deletion

private:
	virtual void act_custom() = 0;
	virtual void deleteLater_custom(); //Not pure virtual, optional behaviour.
};