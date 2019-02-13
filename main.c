#include <stdio.h>
#include <string.h>
#include <unistd.h>

#include <signal.h>
#include <sys/sysinfo.h>

/* libprocps-dev */
#include <proc/readproc.h>

#define INTERVAL 2 /* number of seconds in delay. Tune this to your liking */
#define FREE_RAM_THRESHOLD 1024 * 1024 * 300 /* 300mb */
#define NO_MERCY 1 /* some proccesses take time/give a warning when being SIGTERM'ed. This removes that delay with a few drawbacks */

/* a list of process names that are the usual OOM offender. Customize this to your liking */

char *offenders[] = { /* this list will kill any process with any one of the strings in it's name if it's next up on the chopping block. Doesnt have to be the full name */
	"firefox",
	"Web Content",
	"spotify",
	"Discord",
	"chromium"
};


int main(int argc, char ** argv)
{
	struct sysinfo status;
	PROCTAB* proc = NULL;
	proc_t proccess;
	int i, killed = 0;
	
	memset(&status, 0, sizeof(status));
	
	printf("QSR says \"goodbye OOM pain\"\n");
	
	while(1)
	{
		killed = 0;
		sleep(INTERVAL);
		
		if(sysinfo(&status))
		{
			fprintf(stderr, "could not get system info, please make sure this is running with appropriate privelages\n");
			return 1;
		}
		
		/* handle anything that is too large */
		
		if(status.freeram <= FREE_RAM_THRESHOLD) /* if the amount of free ram is too low */
		{
			fprintf(stderr, "unacceptable ram usage limit reached at %lu\n", status.freeram);
			
			memset(&proccess, 0, sizeof(proccess));
			
			if(!(proc = openproc(PROC_FILLMEM | PROC_FILLSTAT | PROC_FILLSTATUS | PROC_FILLUSR)))
			{
				fprintf(stderr, "error running openproc\n");
				/* return 1; */ /* maybe just wait and see */
			}
			else
			{
				while(readproc(proc, &proccess))
				{
					/* see if there is a proccess close enough to anything in the array */
					for(i = 0; i < sizeof(offenders) / sizeof(char *); i++)
					{
						if(strstr(proccess.cmd, offenders[i]))
						{
							fprintf(stderr, "found %s PID: %d and responding appropriately\n", proccess.cmd, proccess.tid);
							
							kill((pid_t)proccess.tid, NO_MERCY ? SIGKILL : SIGTERM);
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