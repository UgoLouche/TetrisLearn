#include <ctime>

#include "TetrisLogger.hpp"

#include "const.hpp"

TetrisLogger::TetrisLogger(std::string info_path, std::string warning_path, std::string err_path)
{
	mask = NONE;

	if (info_path != "")
	{
		info.open(info_path, std::ostream::app);
		if (info.is_open()) mask |= INFO;
	}

	if (warning_path != "")
	{
		warning.open(warning_path, std::ostream::app);
		if (warning.is_open()) mask |= WARNING;
	}

	if (err_path != "")
	{
		error.open(err_path, std::ostream::app);
		if (error.is_open()) mask |= ERROR;
	}
}

TetrisLogger::~TetrisLogger()
{
	//log(CLOSING_LOG_STR, ALL);

	if (info.is_open()) info.close();
	if (warning.is_open()) warning.close();
	if (error.is_open()) error.close();
}

void TetrisLogger::custom_log(const std::string& message, uint16_t lvl) const
{
	std::string prefix = getPrefix();

	if (lvl & INFO & mask)
		info << prefix << " :: " << message << "\n";

	if (lvl & WARNING & mask)
		warning << prefix << " :: " << message << "\n";

	if (lvl & ERROR & mask)
		error << prefix << " :: " << message << "\n";
}

std::string TetrisLogger::getPrefix() const
{
	time_t raw = time(&raw);
	return std::string(ctime(&raw), 24);
}