#pragma once

#include <cstdint>
#include <string>

#ifdef PROTOENG_EXPORTS
#define PROTOENG_API __declspec(dllexport) 
#else
#define PROTOENG_API __declspec(dllimport) 
#endif

namespace ProtoEng
{
	class SimpleLogger;

	class LoggedObject
	{
	public:
		PROTOENG_API LoggedObject();
		PROTOENG_API LoggedObject(SimpleLogger* logger);
		PROTOENG_API virtual ~LoggedObject();

		PROTOENG_API void linkLogger(SimpleLogger* logger);

	protected:
		PROTOENG_API void log(const std::string& message, uint16_t lvl) const;
		PROTOENG_API void infoLog(const std::string& message) const;
		PROTOENG_API void warnLog(const std::string& message) const;
		PROTOENG_API void errLog(const std::string& message) const;

	private:
		//Can be redefined to write class-specific header
		PROTOENG_API virtual std::string custom_log_header() const;

		SimpleLogger* logger;
	};

}

