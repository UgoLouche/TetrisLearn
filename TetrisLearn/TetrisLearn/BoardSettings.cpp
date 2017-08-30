#include "BoardSettings.hpp"

BoardSettings::BoardSettings() : BoardSettings(InputMethod::Player) {}

BoardSettings::BoardSettings(InputMethod control) : control(control)
{
	switch (control)
	{
	case InputMethod::Player:
		new(&mapping) EnumeratedArray<sf::Keyboard::Key, InputKeys, TOTAL_KEYS>();
		break;
	case InputMethod::Learning:
		new(&fileName) std::string();
		break;
	case InputMethod::Static_AI:
	default:
		throw new std::exception(); //Not implemented yet
		break;
	}
}

BoardSettings::~BoardSettings()
{
	deleteUnion();
}

void BoardSettings::deleteUnion()
{
	switch (control)
	{
	case InputMethod::Player:
		mapping.~EnumeratedArray();
		break;
	case InputMethod::Learning:
		fileName.~basic_string();
		break;
	case InputMethod::Static_AI:
	default:
		throw new std::exception(); //Not implemented yet
		break;
	}
}

