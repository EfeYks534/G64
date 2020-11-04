#pragma once

#include <stdint.h>

typedef struct Vector
{
	int32_t size;
	int32_t top;
	void **array;
} Vector;

Vector *VectorNew(int32_t size); // create new vector

void VectorDelete(Vector *v, int8_t type); // type: 'v' -> deletes the value | other -> doesn't delete the value

int32_t VectorPush(Vector *v, void *el); // pushes a new element to the vector

void *VectorPop(Vector *v); // pops an element from the vector

void *VectorPeek(Vector *v, int32_t i); // gets element i from the vector

void *VectorPopEl(Vector *v, int32_t id); // pop the element i from the vector

void VectorResize(Vector *v, int32_t size); // resize the vector

