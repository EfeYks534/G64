#pragma once

#include <stdint.h>

typedef struct StringBuilder
{
	char    *string;
	uint32_t length;
	uint32_t    top;
} StringBuilder;

StringBuilder *StringBuilderNew();

void StringBuilderAppend(StringBuilder *builder, const char *format, ...);

char *StringBuild(StringBuilder *builder);

void StringBuilderDelete(StringBuilder *builder);

