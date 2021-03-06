#include "HashLib.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

#ifdef __cplusplus
extern "C" {
#endif

uint64_t HashDefaultFunction(uint8_t *data, uint32_t size)
{
	uint64_t hash = 0;
	for(uint32_t i = 0; i < size; i++) {
		hash += ( data[i] + (hash << ( i % 24 + 1 )));
		uint8_t sh = (size + i) % 42 + 1;
		hash ^= (hash << sh) | (hash >> (63 - sh));
	}
	return hash;
}

HashEntry *HashEntryNew(uint8_t *name, int32_t size, void *val)
{
	HashEntry *entry = (HashEntry*) malloc(sizeof(HashEntry));
	*entry = (HashEntry) { size, name, val, NULL };
	return entry;
}

void HashEntryDelete(HashEntry **_entry)
{
	HashEntry *entry = *_entry;

	if(entry == NULL) return;
	HashEntry *last = entry;
	while(entry->next != NULL) {
		last = entry;
		entry->name = entry->next->name;
		entry->value = entry->next->value;
		entry->size = entry->next->size;
		entry = entry->next;
	}

	if(entry != last)
		last->next = NULL;
	else
		*_entry = NULL;
	
	free(entry);
}

HashMap *HashMapNew(int32_t size, uint64_t (*hash_fun) (uint8_t*, uint32_t) )
{
	HashMap *map = (HashMap*) malloc(sizeof(HashMap));
	if(hash_fun == NULL) hash_fun = HashDefaultFunction;
	*map = (HashMap) { size, (HashEntry**) calloc(size, sizeof(HashEntry*)), hash_fun };
	return map;
}

void HashMapDelete(HashMap *map)
{
	for(int i = 0; i < map->size; i++)
		HashEntryDelete(&map->entry[0]);
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
				HashEntryDelete(&map->entry[hash]);
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

#ifdef __cplusplus
}
#endif
