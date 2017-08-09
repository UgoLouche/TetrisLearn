#include "IActingObject.hpp"

IActingObject::IActingObject()
{
}

IActingObject::~IActingObject()
{ }

void IActingObject::Act()
{
	act_custom();
}

void IActingObject::deleteLater()
{
	deleteLater_custom();
}

void IActingObject::deleteLater_custom()
{
}
