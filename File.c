#include "FileLib.h"
#include <sys/types.h>
#include <sys/stat.h>
#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <string.h>

#ifdef __cplusplus
extern "C" {
#endif

File *FileRead(const char *path)
{
	File *file = (File*) malloc(sizeof(File));
	file->size = 0;
	file->data = NULL;

	FILE *f;
	if( (f = fopen(path, "r")) ) {
		fseek(f, 0, SEEK_END);
		file->size = ftell(f);
		fseek(f, 0, SEEK_SET);

		file->data = (char*) malloc(file->size);
		fread(file->data, 1, file->size, f);

		fclose(f);
	}

	return file;
}

void FileWrite(File *file, const char *path)
{
	FILE *f;
	if( (f = fopen(path, "w")) ) {
		fwrite(file->data, 1, file->size, f);
		fclose(f);
	}
}

void FileDelete(File *f)
{
	if(f->data != NULL) free(f->data);
	free(f);
}

int32_t FileSize(const char *path)
{
	FILE *f;
	int32_t size = -1;
	if( (f = fopen(path, "r")) ) {
		fseek(f, 0, SEEK_END);
		size = ftell(f);
		fseek(f, 0, SEEK_SET);
		fclose(f);
	}
	return size;
}

void DirectoryGetContents(const char *path, char **names, size_t *size, size_t max)
{
	struct stat s;
	if(stat(path, &s) == 0) {
		if(!(s.st_mode & S_IFDIR)) return; // Return if `dir` isn't directory
	} else return; // Can't get file stats

	DIR *dir = opendir(path);

	struct dirent *ent = readdir(dir);
	size_t i = 0;
	while(ent != NULL) {
		if(i >= max) break;
		names[i] = (char*)calloc(strlen(ent->d_name) + 2, 1);
		memcpy(names[i], ent->d_name, strlen(ent->d_name));
		names[i][strlen(ent->d_name)+1] = ent->d_type;
		i++;
		ent = readdir(dir);
	}

	*size = i;
	closedir(dir);
}

#ifdef __cplusplus
}
#endif
