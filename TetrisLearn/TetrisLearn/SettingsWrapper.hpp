#pragma once

#include <string>

#include <SFML\Graphics.hpp>

#include "pugiXML\pugixml.hpp"

#include "enum.hpp"
#include "const.hpp"

#include "EnumeratedArray.hpp"
#include "ISettingsWrapper.hpp"


class SettingsWrapper : public ISettingsWrapper
{
public:
	SettingsWrapper();
	~SettingsWrapper();

	bool loadSettings(std::string path);
	bool loadDefault();

private:
	GeneralSettings settings;

	void allocateBoards();
	void clearBoards();

	//Parsing methods for high level markups
	bool parseGame  (const pugi::xml_node&   gameNode);
	bool parseUI    (const pugi::xml_node&     UINode);
	bool parseRules (const pugi::xml_node&  rulesNode);
	bool parseVisual(const pugi::xml_node& visualNode);
	bool parseScore (const pugi::xml_node&  scoreNode);
	bool parseBoards(const pugi::xml_node& boardsNode);

	//Board stuff
	BoardSettings* allocateCustomBoard(const pugi::xml_node& boardNode);
	void resetBoard(BoardSettings& board); //Reset a given board to default value
	bool parseKeyboard(const pugi::xml_node& keyboardNode, BoardSettings& board);
	bool parseLearning(const pugi::xml_node& learningNode, BoardSettings& board);


	//Parsing methods for simple markups
	sf::Vector2i parseSize         (const pugi::xml_node& node, const sf::Vector2i& defaultValue) const;
	int          parseSize         (const pugi::xml_node& node, const int           defaultValue) const;
	sf::Vector2f parsePosition     (const pugi::xml_node& node, const sf::Vector2f& defaultValue) const;
	std::string  parseName         (const pugi::xml_node& node, const std::string&  defaultValue) const;
	std::string  parsePath         (const pugi::xml_node& node, const std::string&  defaultValue) const;
	int          parsePoolsize     (const pugi::xml_node& node, const int           defaultValue) const;
	std::string  parseTexture      (const pugi::xml_node& node, const std::string&  defaultValue) const;
	std::string  parseCounter      (const pugi::xml_node& node, const std::string&  defaultValue) const;
	int          parseLock         (const pugi::xml_node& node, const int           defaultValue) const;
	int          parseAutoScroll   (const pugi::xml_node& node, const int           defaultValue) const;
	int          parseFlash        (const pugi::xml_node& node, const int           defaultValue) const;
	int          parseTimer        (const pugi::xml_node& node, const int           defaultValue) const;
	int          parseStepToEnable (const pugi::xml_node& node, const int           defaultValue) const;
	float        parseScore        (const pugi::xml_node& node, const float         defaultValue) const;
	float        parseMultiplier   (const pugi::xml_node& node, const float         defaultValue) const;
	float        parseSpeed        (const pugi::xml_node& node, const float         defaultValue) const;
	float        parseLine         (const pugi::xml_node& node, const float         defaultValue) const;

	// Underlying type parsing
	sf::Keyboard::Key parseKey    (const pugi::xml_node& node, const sf::Keyboard::Key& defaultValue) const;
	int               parseInt    (const pugi::xml_node& node, const int                defaultValue) const;
	float             parseFloat  (const pugi::xml_node& node, const float              defaultValue) const;
	std::string       parseString (const pugi::xml_node& node, const std::string&       defaultValue) const;
	sf::Color         parseColor  (const pugi::xml_node& node, const sf::Color&         defaultValue) const;
	sf::Vector2f      parseVector (const pugi::xml_node& node, const sf::Vector2f&      defaultValue) const;
	sf::Vector2i      parseVector (const pugi::xml_node& node, const sf::Vector2i&      defaultValue) const;


	//Misc
	void boardsSwap(size_t indexA, size_t indexB);

	// Inherited via ISettingsWrapper
	virtual const GeneralSettings & exposeData() const override;
};