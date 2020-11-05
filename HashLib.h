#pragma once

#include <stdint.h>

#define HASH_MAP_SIZE 100

typedef struct HashEntry
{
	int32_t           size; // Entry name size
	uint8_t          *name; // Entry name (raw data)
	void            *value; // Entry value
	struct HashEntry *next; // This is a linked list
} HashEntry;

HashEntry *HashEntryNew(uint8_t *name, int32_t size, void *val); // new hash entry

void HashEntryDelete(HashEntry *entry); // delete hash entry

typedef struct HashMap
{
	int32_t        size;
	HashEntry   **entry;
	uint64_t (*hash_fun) (uint8_t*, uint32_t);
} HashMap;

HashMap *HashMapNew(int32_t size, uint64_t (*hash_fun) (uint8_t*, uint32_t) ); // new hash map

void HashMapDelete(HashMap *map); // delete hash map

void HashPut(HashMap *map, uint8_t *name, int32_t size, void *value); // put the value to hashmap

void HashDelete(HashMap *map, uint8_t *name, int32_t size); // delete the corresponding hash entry

void *HashFind(HashMap *map, uint8_t *name, int32_t size); // find the corresponding value

HashEntry *HashFindEntry(HashMap *map, uint8_t *name, int32_t size); // find the corresponding entry


