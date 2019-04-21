#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>

/* libprocps-dev */
#include <proc/sysinfo.h>
#include <proc/readproc.h>

// internal includes
#include "cparser.h"
#include "ulog.h"
#include "strutil.h"

// add or remove this if you're testing without installing
//#define _TESTING

char **offenders;
unsigned offenders_size = 0;

unsigned interval = 0;
unsigned free_threshold = 0;
bool mercy;

enum defaults_mode_e {
	DF_MODE_NIL, // dont load anything
	DF_MODE_OFFENDERS,
	DF_MODE_INTERVAL,
	DF_MODE_THRSH,
	DF_MODE_MERCY
};

void load_defaults(unsigned mode){
	if(mode & DF_MODE_NIL) return; // someone was eating the leg
	ulog_log_message("Loading default configuration", LOG_INFO);

	if(mode & DF_MODE_OFFENDERS) {
		offenders = (char**)calloc(5, sizeof(char*));
		offenders[0] = "firefox";
		offenders[1] = "Web Content";
		offenders[2] = "spotify";
		offenders[3] = "Discord";
		offenders[4] = "chromium";
		offenders_size = 5;
	}

	if(mode & DF_MODE_INTERVAL) interval = 2;
	if(mode & DF_MODE_THRSH) free_threshold = 300;
	if(mode & DF_MODE_MERCY) mercy = false;
}


int load_config(char* filename){
	FILE* fp = fopen(filename, "rb");
	if(fp == NULL){
		ulog_log_message("Error opening qsr.conf!", LOG_ERROR);
		return 1;
	} else {
		config_parse_file(fp);
	}

	// Get and load data from configuration file

	char* item = config_get_item("qsr_interval");
	if(!strcmp(item, "")) {
		ulog_log_message("Interval is empty or non-existant in config file", LOG_WARN);
		load_defaults(DF_MODE_INTERVAL);
	} else {
		// convert the item data so we can use it
		int interval_atoi = atoi(item);
		if(interval_atoi == -1) {
			ulog_log_message("Interval is invalid in config file", LOG_WARN);
			load_defaults(DF_MODE_INTERVAL);
		} else {
			interval = interval_atoi;
		}
	}

	item = config_get_item("qsr_freeramtsh");
	if(!strcmp(item, "")) {
		ulog_log_message("RAM threshold is empty or non-existant in config file", LOG_WARN);
		load_defaults(DF_MODE_THRSH);
	} else {
		// convert the item data so we can use it
		int threshold_atoi = atoi(item);
		if(threshold_atoi == -1) {
			ulog_log_message("RAM threshold is invalid in config file", LOG_WARN);
			load_defaults(DF_MODE_THRSH);
		} else {
			free_threshold = threshold_atoi;
		}
	}

	item = config_get_item("qsr_mercy");
	if(!strcmp(item, "")) {
		ulog_log_message("Mercy value is empty or non-existant in config file", LOG_WARN);
		load_defaults(DF_MODE_MERCY);
	} else {
		// convert the item data so we can use it
		int mercy_atoi = atoi(item);
		if(mercy_atoi == -1) {
			ulog_log_message("Mercy value is invalid in config file", LOG_WARN);
			load_defaults(DF_MODE_MERCY);
		} else {
			mercy = mercy_atoi;
		}
	}

	item = config_get_item("qsr_offenders");
	if(!strcmp(item, "")) {
		ulog_log_message("Offenders are empty or non-existant in config file", LOG_WARN);
		load_defaults(DF_MODE_OFFENDERS);
	} else {
		int array_size = 0;
		char* pass1_ptr = (char*)calloc(1, 128);
		strcpy(pass1_ptr, item);
		
		char* pass1_tok;
		while((pass1_tok = strtok_r(pass1_ptr, ",", &pass1_ptr))) {
			array_size++;
		}

		// allocate array
		offenders = (char**)calloc(array_size, sizeof(char*));
		offenders_size = array_size;
	
		// Final pass is actually putting values in offenders
		char* pass2_ptr = item;
		char* pass2_tok;
		int pass2_i = 0;
		while((pass2_tok = strtok_r(pass2_ptr, ",", &pass2_ptr))) {
			trim_leading(pass2_tok);
			trim_trailing(pass2_tok);
			offenders[pass2_i] = pass2_tok;
			pass2_i++;
		}
	}

	return 0;
}

/* Clean up dynamically allocated data */
void cleanup(){
	free(offenders);
}

int main(int argc, char ** argv) {
	atexit(cleanup);
	PROCTAB* proc = NULL;
	bool killed;
	proc_t proccess;

#ifdef _TESTING
	if(load_config("qsr.conf")){
#else
	if(load_config("/usr/share/qsr/qsr.conf")){
#endif
		return 1;
	}

	ulog_log_message("QSR says \"goodbye OOM pain\"", LOG_INFO);
	
	while(1) {
		killed = false;
		sleep(interval);

		meminfo();
		int mb_free = (kb_main_available / 1024);

		/* if the amount of free ram is too low */
		if(mb_free <= free_threshold) {

			char* logfmt = (char*)calloc(1, 128);
			sprintf(logfmt, "Free ram has hit threshold (Free ram is %dMB)", mb_free);
			ulog_log_message(logfmt, LOG_INFO);
			free((char*)logfmt);
			
			memset(&proccess, 0, sizeof(proccess));
			
			if(!(proc = openproc(PROC_FILLMEM | PROC_FILLSTAT | PROC_FILLSTATUS | PROC_FILLUSR))) {
				ulog_log_message("openproc() failed", LOG_ERROR);
			} else {

				while(readproc(proc, &proccess)) {
					
					// check if there is a process with a matching or close name
					// to any in the offenders array
					for(int i = 0; i < offenders_size; i++) {
						if(strstr(proccess.cmd, offenders[i])){
							char* logfmt = (char*)calloc(1, 128); // just in case
							sprintf(logfmt, "Killing %s (PID %d)", proccess.cmd, proccess.tid);
							ulog_log_message(logfmt, LOG_INFO);
							free((char*)logfmt);
							
							kill((pid_t)proccess.tid, mercy ? SIGTERM : SIGKILL);
							killed = 1;
							break;
						}
					}
					if(killed)
						break;
				}
				
				closeproc(proc);
			}
		}
	}
}
