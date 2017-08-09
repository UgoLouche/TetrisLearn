#include <exception>

#include "ISettingsWrapper.hpp"

ISettingsWrapper::~ISettingsWrapper()
{
}

const GeneralSettings & ISettingsWrapper::getSettings() const
{
	return exposeData();
}

const BoardSettings & ISettingsWrapper::getBoard(size_t index) const
{
	if (index >= exposeData().boardCount)
		throw std::exception();
	else if (exposeData().boardsArray[index] == nullptr)
		return exposeData().defaultBoard;
	else
		return *(exposeData().boardsArray[index]);
}