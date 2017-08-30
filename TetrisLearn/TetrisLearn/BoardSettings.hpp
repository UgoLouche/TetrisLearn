#pragma once

#include <string>

#include "SFML\Graphics.hpp"

#include "enum.hpp"
#include "EnumeratedArray.hpp"
#include "const.hpp"


struct BoardSettings
{
public:
	InputMethod control;
	std::string name;

	bool recording;
	bool autoReload;

	//Constr / Destr because of union.
	BoardSettings();
	BoardSettings(InputMethod control);

	~BoardSettings();

	template<typename T>
	T& getUnion();

	template<typename T>
	T& getUnion() const;

	template<typename T>
	void setUnion(T newVal);

	//Delte copy/Move, uneeded
	BoardSettings(const BoardSettings& cpy) = delete;
	BoardSettings& operator=(const BoardSettings& cpy) = delete;

private:
	union
	{
		EnumeratedArray<sf::Keyboard::Key, InputKeys, TOTAL_KEYS> mapping;
		std::string fileName;
	};

	void deleteUnion();

};

template<>
inline const EnumeratedArray<sf::Keyboard::Key, InputKeys, TOTAL_KEYS>&  BoardSettings::getUnion<const EnumeratedArray<sf::Keyboard::Key, InputKeys, TOTAL_KEYS>>() const
{
	if (control == InputMethod::Player)
		return mapping;
	else
		throw new std::exception();
}

template<>
inline EnumeratedArray<sf::Keyboard::Key, InputKeys, TOTAL_KEYS>&  BoardSettings::getUnion<EnumeratedArray<sf::Keyboard::Key, InputKeys, TOTAL_KEYS>>()
{
	using EnumArray = EnumeratedArray<sf::Keyboard::Key, InputKeys, TOTAL_KEYS>;

	return const_cast<EnumArray&>( static_cast<const BoardSettings&>(*this).getUnion<const EnumArray>() );
}

template<>
inline const std::string& BoardSettings::getUnion<const std::string>() const
{
	if (control == InputMethod::Learning)
		return fileName;
	else
		throw new std::exception();
}

template<>
inline std::string& BoardSettings::getUnion<std::string>()
{
	return const_cast<std::string&>( static_cast<const BoardSettings&>(*this).getUnion<const std::string>());
}

template<>
inline void BoardSettings::setUnion<EnumeratedArray<sf::Keyboard::Key, InputKeys, TOTAL_KEYS>>(EnumeratedArray<sf::Keyboard::Key, InputKeys, TOTAL_KEYS> newVal)
{
	deleteUnion();

	if (control != InputMethod::Player) control = InputMethod::Player;

	this->mapping = newVal;
}

template<>
inline void BoardSettings::setUnion<std::string>(std::string newVal)
{
	deleteUnion();

	if (control != InputMethod::Learning) control = InputMethod::Learning;

	this->fileName = newVal;
}