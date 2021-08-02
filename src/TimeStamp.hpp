#ifndef TIMESTAMP_HPP_
#define TIMESTAMP_HPP_

#include "Config.hpp"

#include <chrono>
#include <ctime>
#include <sstream>
#include <string>


namespace Utils
{
	/**
	 *
	 */
	inline std::string TimeStamp()
	{
		auto microsecs = std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::system_clock::now().time_since_epoch()).count();

		std::time_t timer  = static_cast<std::time_t>(microsecs / 1'000'000);
		struct tm tm;
		localtime_r(&timer, &tm); // thread safe version of std::localtime

		std::ostringstream os;

		os << tm.tm_year+1900 << "-"
			<< tm.tm_mon+1  << "-"
			<< tm.tm_mday  << "-"
			<< tm.tm_hour << "-"
			<< tm.tm_min << "-"
			<< tm.tm_sec << "-"
			<< static_cast<int>(microsecs % 1'000'000);
		return os.str();
	}
} /* namespace Utils */

#endif // SRC_TIMESTAMP_HPP_ 
