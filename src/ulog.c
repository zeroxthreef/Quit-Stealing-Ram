#include "ulog.h"
#include <stdlib.h>
#include <time.h>

// this is nix only so hey why not
#define RESETCOLOR "\033[0m"
#define WCOLOR "\033[33;1m"
#define ECOLOR "\033[31;1m"

// TODO: I need to test memleaks in this
// (valgrind *didn't* say anything about this function but better to be safe than ouch)
void ulog_log_message(const char* message, int severity){
	switch(severity){
		default:break;
		
		case LOG_INFO: {
			char* timestr = (char*)calloc(1, 128);
			time_t timey = time(NULL);
			struct tm* timeboi = localtime(&timey);
			strftime(timestr, 128, "%F %k:%M", timeboi);
			printf("[Info (%s)] %s\n", timestr, message);
			free((char*)timestr);
		} break;

		case LOG_WARN: {
			char* timestr = (char*)calloc(1, 128);
			time_t timey = time(NULL);
			struct tm* timeboi = localtime(&timey);
			strftime(timestr, 128, "%F %k:%M", timeboi);
			printf("%s[Warning (%s)] %s%s\n", WCOLOR, timestr, message, RESETCOLOR);
			free((char*)timestr);
		} break;

		case LOG_ERROR: {
			char* timestr = (char*)calloc(1, 128);
			time_t timey = time(NULL);
			struct tm* timeboi = localtime(&timey);
			strftime(timestr, 128, "%F %k:%M", timeboi);
			fprintf(stderr, "%s[Error (%s)] %s%s\n", ECOLOR, timestr, message, RESETCOLOR);
			free((char*)timestr);
		} break;
	}
}
