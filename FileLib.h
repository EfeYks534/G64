#pragma once

#include <stdint.h>

typedef struct File
{
	char   *data;
	int32_t size;
} File;

File *FileRead(const char *path); // Read the file from path

void FileWrite(File *f, const char *path); // Write file to path

void FileDelete(File *f); // Delete file

int32_t FileSize(const char *path); // Size of path

