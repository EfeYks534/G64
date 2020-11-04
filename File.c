#include "FileLib.h"
#include <stdio.h>
#include <stdlib.h>

File *FileRead(const char *path)
{
	File *file = malloc(sizeof(File));
	file->size = 0;
	file->data = NULL;

	FILE *f;
	if( (f = fopen(path, "r")) ) {
		fseek(f, 0, SEEK_END);
		file->size = ftell(f);
		fseek(f, 0, SEEK_SET);

		file->data = malloc(file->size);
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

