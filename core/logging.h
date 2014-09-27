
#ifndef _NESOX_LOGGING_
#define _NESOX_LOGGING_

#include <iomanip>
#include <fstream>
#include <iostream>
#include <string>

enum LoggingSeverity { INFO, WARNING, ERROR, FATAL };

class Logging {
private:
	LoggingSeverity severity;
	static std::ofstream info_log_stream;
	static std::ofstream warn_log_stream;
	static std::ofstream erro_log_stream;
	friend void InitializeLogger(const std::string& info_log_file, const std::string& warn_log_file, const std::string& erro_log_file);
public:
	static std::ostream& GetStream(LoggingSeverity severity);
	static std::ostream& Start(LoggingSeverity severity, const std::string& file, int line, const std::string& function);
public:
  Logging(LoggingSeverity s) : severity(s) {}
  ~Logging();
};

void InitializeLogger(const std::string& info_log_file, const std::string& warn_log_file, const std::string& erro_log_file);

#define LOG(severity) Logging(severity).Start(severity, __FILE__, __LINE__, __FUNCTION__)
#endif /*_NESOX_LOGGING_*/
