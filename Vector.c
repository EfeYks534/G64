#include "VectorLib.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

Vector *VectorNew(int32_t size)
{
	Vector *v = malloc(sizeof(Vector));
	*v = (Vector) { size, 0, calloc(size, sizeof(void*)) };
	return v;
}

void VectorDelete(Vector *v, int8_t type)
{
	if(type == 'v') 
		for(int i = 0; i < v->top; i++)
			free(v->array[i]);
	free(v->array);
	free(v);
}

int VectorPush(Vector *v, void *el)
{
	if(v->top >= v->size) VectorResize(v, v->size+10);
	int32_t id = v->top++;
	v->array[id] = el;
	return id;
}

void *VectorPop(Vector *v)
{
	if(v->top == 0) return NULL;
	void *el = v->array[--v->top];
	v->array[v->top] = NULL;
	VectorResize(v, v->size-1);
	return el;
}

void *VectorPeek(Vector *v, int32_t i)
{
	return v->array[i];
}

void *VectorPopEl(Vector *v, int32_t id)
{
	void *el = v->array[id];
	v->array[id] = NULL;
	for(int i = id; i < v->top-2; i++) v->array[i] = v->array[i+1];
	return el;
}

void VectorResize(Vector *v, int32_t size)
{
	if(size <= 0) return;
	int32_t size_last = v->size;
	v->size = size;
	v->array = realloc(v->array, size*sizeof(void*));
	if(size > size_last)
		memset(&(v->array[size_last]), 0, size-size_last);
}

