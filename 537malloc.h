// Kenny Jin jjin59@wisc.edu
// Ying Zhang yzhang865@wisc.edu

#ifndef MALLOC537
#define MALLOC537

#include "tree.h"

#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>

RangeTree rangeTree;

void *malloc537(size_t size);

void free537(void *ptr);

void *realloc537(void *ptr, size_t size);

void memcheck537(void *ptr, size_t size);

#endif