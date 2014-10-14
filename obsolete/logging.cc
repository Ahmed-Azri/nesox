
#include "logging.h"

#include <stdlib.h>
#include <time.h>

std::ofstream Logging::info_log_stream;
std::ofstream Logging::warn_log_stream;
std::ofstream Logging::erro_log_stream;

void InitializeLogger(const std::string& info_log_file, const std::string& warn_log_file, const std::string& erro_log_file){
	Logging::info_log_stream.open(info_log_file.c_str());
	Logging::warn_log_stream.open(warn_log_file.c_str());
	Logging::erro_log_stream.open(erro_log_file.c_str());
}

std::ostream& Logging::GetStream(LoggingSeverity severity){
	switch(severity){
		case INFO: return ((info_log_stream.is_open()) ? info_log_stream : std::cout); break;
		case WARNING: return ((warn_log_stream.is_open()) ? warn_log_stream : std::cerr); break;
		default: return ((erro_log_stream.is_open()) ? erro_log_stream : std::cerr); break;
	}
}

std::ostream& Logging::Start(LoggingSeverity severity, const std::string& file, int line, const std::string& function){
	time_t tm;
	time(&tm);
	char time_string[0xff];
	ctime_r(&tm, time_string);
	return GetStream(severity) << time_string << file << ":" << line << " (" << function << "): " << std::flush;
}


Logging::~Logging(){
	GetStream(severity) << "\n" << std::flush;

	if (severity == FATAL) {
		Logging::info_log_stream.close();
		Logging::warn_log_stream.close();
		Logging::erro_log_stream.close();
		abort();
	}
}

