#pragma once

#include <fstream>
#include <string>

#include "ProtoEng\SimpleLogger.h"

class TetrisLogger : public ProtoEng::SimpleLogger
{
public:
	TetrisLogger(std::string info_path = "", std::string warning_path = "", std::string err_path = "");
	~TetrisLogger();

private:
	virtual void custom_log(const std::string& message, uint16_t lvl) const override;

	std::string getPrefix() const;

	mutable std::ofstream info;
	mutable std::ofstream warning;
	mutable std::ofstream error;

	uint16_t mask;
};