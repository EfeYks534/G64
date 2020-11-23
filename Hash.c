#include "HashLib.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

static uint64_t HashDefaultFunction(uint8_t *data, uint32_t size)
{
	uint64_t hash = 0;
	for(int i = 0; i < size; i++) {
		hash = ( data[i] + (hash << ( i % 63 + 1 )) + hash );
		uint8_t sh = (size + i) % 63 + 1;
		hash = (hash >> sh) | (hash << ( (1 << 3) - sh ) );
	}
	return hash;
}

HashEntry *HashEntryNew(uint8_t *name, int32_t size, void *val)
{
	HashEntry *entry = malloc(sizeof(HashEntry));
	*entry = (HashEntry) { size, name, val, NULL };
	return entry;
}

void HashEntryDelete(HashEntry *entry)
{
	if(entry == NULL) return;
	HashEntry *last = entry;
	while(entry->next != NULL) {
		last = entry;
		entry->name = entry->next->name;
		entry->value = entry->next->value;
		entry->size = entry->next->size;
		entry = entry->next;
	}
	free(entry);
	if(entry != last)
		last->next = NULL;
}

HashMap *HashMapNew(int32_t size, uint64_t (*hash_fun) (uint8_t*, uint32_t) )
{
	HashMap *map = malloc(sizeof(HashMap));
	if(hash_fun == NULL) hash_fun = HashDefaultFunction;
	*map = (HashMap) { size, calloc(size, sizeof(HashEntry*)), hash_fun };
	return map;
}

void HashMapDelete(HashMap *map)
{
	for(int i = 0; i < map->size; i++)
		HashEntryDelete(map->entry[i]);
	free(map->entry);
	free(map);
}

void HashPut(HashMap *map, uint8_t *name, int32_t size, void *value)
{
	uint64_t hash = map->hash_fun(name, size) % map->size;
	HashEntry *entry = map->entry[hash];
	if(entry == NULL) {
		map->entry[hash] = HashEntryNew(name, size, value);
		return;
	}
	while(1) {
		if(size == entry->size)
			if(memcmp(name, entry->name, size) == 0) {
				entry->value = value;
				return;
			}
		if(entry->next == NULL) {
			entry->next = HashEntryNew(name, size, value);
			return;
		}
		entry = entry->next;
	}
}
void HashDelete(HashMap *map, uint8_t *name, int32_t size)
{
	uint64_t hash = map->hash_fun(name, size) % map->size;
	HashEntry *entry = map->entry[hash];
	if(entry == NULL) return;
	while(1) {
		if(size == entry->size)
			if(memcmp(name, entry->name, size) == 0) {
				HashEntryDelete(entry);
				return;
			}
		if(entry->next == NULL) return;
		entry = entry->next;
	}
}

void *HashFind(HashMap *map, uint8_t *name, int32_t size)
{
	HashEntry *entry = HashFindEntry(map, name, size);
	if(entry == NULL) return NULL;
	return entry->value;
}

HashEntry *HashFindEntry(HashMap *map, uint8_t *name, int32_t size)
{
	uint64_t hash = map->hash_fun(name, size) % map->size;
	HashEntry *entry = map->entry[hash];
	if(entry == NULL) return NULL;
	while(1) {
		if(size == entry->size)
			if(memcmp(name, entry->name, size) == 0) break;
		if(entry->next == NULL) return NULL;
		entry = entry->next;
	}
	return entry;
}

