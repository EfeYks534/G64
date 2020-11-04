#include "HashLib.h"
#include <string.h>
#include <stdlib.h>
#include <limits.h>

HashEntry *HashEntryNew(int32_t size, uint8_t *name, void *val)
{
	HashEntry *entry = malloc(sizeof(HashEntry));
	*entry = (HashEntry) { size, name, val, NULL };
	return entry;
}

void HashEntryDelete(HashEntry *entry, int8_t type)
{
	switch(type)
	{
	case '*':
		free(entry->name);
		free(entry->value);
		break;
	case 'v':
		free(entry->value);
		break;
	case 'n':
		free(entry->name);
		break;
	default: break;
	}
	while(entry->next != NULL) {
		entry->size = entry->next->size;
		entry->name = entry->next->name;
		entry->value = entry->next->value;
		entry = entry->next;
	}
	free(entry);
}

HashMap *HashMapNew(int32_t size)
{
	HashMap *map = malloc(sizeof(HashMap));
	*map = (HashMap) { size, calloc(size, sizeof(HashEntry*)) };
	return map;
}

void HashMapDelete(HashMap *map, int8_t type)
{
	while(map->entry[0] != NULL) HashEntryDelete(map->entry[0], type);
	free(map->entry);
	free(map);
}

void HashPut(HashMap *map, int32_t size, uint8_t *name, void *value)
{
	uint64_t hash = GetHash( name, size) % map->size;
	HashEntry *entry = map->entry[hash];
	if(entry == NULL) {
		map->entry[hash] = HashEntryNew(size, name, value);
		return;
	}
	while(1) {
		if(size == entry->size)
			if(memcmp(name, entry->name, size) == 0) {
				entry->value = value;
				return;
			}
		if(entry->next == NULL) {
			entry->next = HashEntryNew(size, name, value);
			return;
		}
		entry = entry->next;
	}
}
void HashDelete(HashMap *map, int32_t size, uint8_t *name, char type)
{
	uint64_t hash = GetHash(name, size) % map->size;
	HashEntry *entry = map->entry[hash];
	if(entry == NULL) return;
	while(1) {
		if(size == entry->size)
			if(memcmp(name, entry->name, size) == 0) {
				HashEntryDelete(entry, type);
				return;
			}
		if(entry->next == NULL) return;
		entry = entry->next;
	}
}

void *HashFind(HashMap *map, int32_t size, uint8_t *name)
{
	HashEntry *entry = HashFindEntry(map, size, name);
	if(entry == NULL) return NULL;
	return entry->value;
}

HashEntry *HashFindEntry(HashMap *map, int32_t size, uint8_t *name)
{
	uint64_t hash = GetHash(name, size) % map->size;
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

