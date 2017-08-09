#include "Settings.hpp"

 ISettingsWrapper const* Settings::wrapper = nullptr;

const GeneralSettings & Settings::get()
{
	return wrapper->getSettings();
}

const BoardSettings & Settings::getBoard(size_t index)
{
	return wrapper->getBoard(index);
}

void Settings::link(const ISettingsWrapper & wrapper)
{
	if (Settings::wrapper == nullptr)  Settings::wrapper = &wrapper;
}
