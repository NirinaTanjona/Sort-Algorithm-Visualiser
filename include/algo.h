#ifndef ALGO_H

#define ALGO_H

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>


typedef struct Array
{
    size_t *items;
    size_t count;
    size_t capacity;
} Array;


void* heapSort(void* arg);
void* bubbleSort(void* arg);
void* mergeSort(void* arg);
void* radixSort(void* arg);

#endif // ALGO_H
