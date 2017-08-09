#pragma once

#include <string>

#include "SFML\Graphics.hpp"

#include "enum.hpp"
#include "const.hpp"
#include "EnumeratedArray.hpp"
#include "ISettingsWrapper.hpp"


class Settings
{
public:
	static const GeneralSettings& get();
	static const BoardSettings& getBoard(size_t index);

	static void link(const ISettingsWrapper& wrapper);

private:
	static ISettingsWrapper const* wrapper;
};