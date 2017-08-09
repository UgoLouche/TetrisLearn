#pragma once

#include <string>
#include <cstdint>

#ifdef PROTOENG_EXPORTS
#define PROTOENG_API __declspec(dllexport) 
#else
#define PROTOENG_API __declspec(dllimport) 
#endif

namespace ProtoEng
{
	class SimpleLogger
	{
	public:
		//Static
		static const uint16_t INFO = 0x0001;
		static const uint16_t WARNING = 0x0002;
		static const uint16_t ERROR = 0x0004;
		static const uint16_t NONE = 0x0000;
		static const uint16_t ALL = INFO | WARNING | ERROR;

		//Local
		PROTOENG_API virtual ~SimpleLogger();

		PROTOENG_API void log(const std::string& message, uint16_t lvl) const;

	private:
		PROTOENG_API virtual void custom_log(const std::string& message, uint16_t lvl) const = 0;
	};
}