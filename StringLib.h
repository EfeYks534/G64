#pragma once

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct StringBuilder
{
	char    *string;
	uint32_t length;
	uint32_t    top;
} StringBuilder;

StringBuilder *StringBuilderNew(); // Make new string builder

void StringBuilderAppend(StringBuilder *builder, const char *format, ...); // Append to string builder

char *StringBuild(StringBuilder *builder); // Build string from string builder

void StringBuilderDelete(StringBuilder *builder); // Delete string builder

#ifdef __cplusplus
}
#endif
