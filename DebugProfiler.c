#include "DebugLib.h"
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <G64/TimeLib.h>

static int   *profile_lines;
static char **profile_files;
static char      **profiles;
static int    profile_index;

static void EndProfileB();
static void Handler(int sig, siginfo_t *info, void *ptr)
{
	printf("\n\n\nMemory fault at %p\n\n\n## STACK TRACE ##\n\n\n", info->si_addr);
	while(profile_index > 0)
		EndProfileB();
	printf("\n");
	exit(-1);
}

void InitDebugger()
{
	profiles = malloc(sizeof(char*)*1024);
	profile_lines = malloc(sizeof(int)*1024);
	profile_files = malloc(sizeof(char*)*1024);
	profile_index = 0;
	struct sigaction act;
	memset(&act, 0, sizeof(struct sigaction));
	act.sa_sigaction = &Handler;
	sigaction(SIGSEGV, &act, NULL);
	sigaction(SIGABRT, &act, NULL);
	sigaction(SIGBUS, &act, NULL);
}

void _StartProfile(char *name, char *file, int line)
{
	SetTimeDiff(profile_index);
	profiles[profile_index++] = name;
	profile_files[profile_index-1] = file;
	profile_lines[profile_index-1] = line;
	//printf("Debugger: %s (file %s, line %d) started.\n", name, file, line);
}

void EndProfile()
{
	--profile_index;
	//printf("Debugger: %s (file %s, line %d) ended. (Took %f seconds.)\n", name,
	//			profile_files[profile_index], profile_lines[profile_index], diff);
}

static void EndProfileB()
{
	double diff = GetTimeDiff(--profile_index);
	char *name = profiles[profile_index];
	printf("Debugger: %s (file %s, line %d) ended. (Took %f seconds.)\n", name,
				profile_files[profile_index], profile_lines[profile_index], diff);
}
