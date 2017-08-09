#include "UIManager.hpp"

#include "SFML\Graphics.hpp"

#include "const.hpp"
#include "Settings.hpp"

UIManager::UIManager()
{
	font.loadFromFile(Settings::get().fontPath);
}

const sf::Font& UIManager::getFont() const
{
	return font;
}
