#pragma once

void InitDebugger();

void _StartProfile(char *name, char *file, int line);

void EndProfile();

#define StartProfile(name) _StartProfile(name, __FILE__, __LINE__)
