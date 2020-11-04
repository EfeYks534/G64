#include "StringLib.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>

StringBuilder *StringBuilderNew()
{
	StringBuilder *builder = malloc(sizeof(StringBuilder));
	builder->string = calloc(256, 1);
	builder->length = 256;
	builder->top = 0;
	return builder;
}

void StringBuilderAppend(StringBuilder *builder, const char *format, ...)
{
	int buffer_size = 0;

	va_list vlist;
	va_start(vlist, format);
	buffer_size = vsnprintf(NULL, 0, format, vlist);
	va_end(vlist);

	buffer_size++;

	char *buf = calloc(buffer_size, 1);

	va_start(vlist, format);
	vsnprintf(buf, buffer_size, format, vlist);
	va_end(vlist);

	for(int i = 0; i < buffer_size-1; i++) {
		builder->string[builder->top++] = buf[i];
		if(builder->top >= builder->length) {
			builder->length *= 2;
			builder->string = realloc(builder->string, builder->length);
		}
	}
}

char *StringBuild(StringBuilder *builder)
{
	char *str = calloc(builder->top+1, 1);
	memcpy(str, builder->string, builder->top);
	str[builder->top] = 0;
	return str;
}

void StringBuilderDelete(StringBuilder *builder)
{
	free(builder->string);
	free(builder);
}
