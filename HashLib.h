#pragma once

#include <stdint.h>

#define HASH_MAP_SIZE 8192

uint64_t GetHash(const uint8_t *data, uint32_t size);

typedef struct HashEntry
{
	int32_t           size; // Entry name size
	uint8_t          *name; // Entry name (raw data)
	void            *value; // Entry value
	struct HashEntry *next; // This is a linked list
} HashEntry;

HashEntry *HashEntryNew(int32_t size, uint8_t *name, void *val); // new hash entry

// type : '*' -> frees all the resources | 'v' -> frees the value | 'n' -> frees the name | other -> don't free
void HashEntryDelete(HashEntry *entry, int8_t type);

typedef struct HashMap
{
	int32_t size;
	HashEntry **entry;
} HashMap;

HashMap *HashMapNew(int32_t size); // new hash map

// type : '*' -> frees all the resources | 'v' -> frees the values | 'n' -> frees the names | other -> don't free
void HashMapDelete(HashMap *map, int8_t type);

void HashPut(HashMap *map, int32_t size, uint8_t *name, void *value); // put the value to hashmap

// type : '*' -> frees all the resources | 'v' -> frees the values | 'n' -> frees the names | other -> don't free
void HashDelete(HashMap *map, int32_t size, uint8_t *name, char type); // delete the corresponding hash entry

void *HashFind(HashMap *map, int32_t size, uint8_t *name); // find the corresponding value

HashEntry *HashFindEntry(HashMap *map, int32_t size, uint8_t *name); // find the corresponding entry


