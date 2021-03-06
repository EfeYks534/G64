#pragma once

#include <stddef.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct File
{
	char   *data;
	int32_t size;
} File;

File *FileRead(const char *path); // Read the file from path

void FileWrite(File *f, const char *path); // Write file to path

void FileDelete(File *f); // Delete file

int32_t FileSize(const char *path); // Size of path

void DirectoryGetContents(const char *dir, char **names, size_t *size, size_t max); // Get a directory's contents

#ifdef __cplusplus
}
#endif
