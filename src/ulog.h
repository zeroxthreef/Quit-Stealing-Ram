#include <stdio.h>

enum ulog_sev_e {
	LOG_INFO,
	LOG_WARN,
	LOG_ERROR // should color and also put to stdERR
};

void ulog_log_message(const char* message, int severity);
