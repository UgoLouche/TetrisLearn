# pragma once

#include "SFML\Graphics.hpp"

class UIManager
{
public:
	UIManager();
	const sf::Font& getFont() const;

private:
	sf::Font font;
};