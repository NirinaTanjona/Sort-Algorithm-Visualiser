#include "algo.h"
#include "raylib.h"
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#define                                                DEFAULT_MIN 1
#define                                             DEFAULT_MAX 100
#define                                              DEFAULT_SIZE 100


typedef enum
{
    MERGESORT,
    BUBBLESORT,
    HEAPSORT,
    RADIXSORT
} AlgoType;


#define da_append(xs, x)\
    do {\
        if (xs->count >= xs->capacity) {\
            if (xs->capacity == 0) xs->capacity = 256;\
            else xs->capacity *= 2;\
            xs->items = realloc(xs->items, xs->capacity*sizeof(*xs->items));\
        }\
        xs->items[xs->count++] = x;\
    } while(0)


void init(Array *xs)
{
    for (int i = 0; i < DEFAULT_SIZE; ++i)
    {
        size_t val = rand()*time(0) % (DEFAULT_MAX - DEFAULT_MIN + 1) + DEFAULT_MIN;
        da_append(xs, val);
    }
}


void customSamples(Array *xs, size_t min, size_t max, size_t size)
{
    xs->count = 0;
    xs->capacity = 0;
    xs->items = realloc(xs->items, 0);
    for (size_t i = 0; i < size; ++i)
    {
        size_t val = rand()*time(NULL) % (max - min + 1) + min;
        da_append(xs, val);
    }
}


void* draw(void* arg)
{
    Array *arr = arg;

    const int screenWidth = 800;
    const int screenHeight = 600;

    InitWindow(screenWidth, screenHeight, "Raylib Window");

    int w = GetRenderWidth();
    int h = GetRenderHeight();
    int cell_width = (float)w / arr->count;

    SetTargetFPS(60);
    while(!WindowShouldClose())
    {
        BeginDrawing();

            for (size_t i = 0; i < arr->count; ++i) {
                float t = (float)arr->items[i] / DEFAULT_MAX;
                DrawRectangle(i*cell_width, h - h/1.2*t, cell_width-2, h*t, BLUE);
            }
            ClearBackground(CLITERAL(Color) {0x18, 0x18, 0x18, 0xFF});

        EndDrawing();
    }
    CloseWindow();
}


int main(void)
{
    int err;
    pthread_t t_heapsort, t_bubblesort, t_mergesort, tdraw, t_radixsort;
    void *tret;

    AlgoType algo;

    Array arr = {0};
    init(&arr);

    algo = RADIXSORT ;

    err = pthread_create(&tdraw, NULL, draw, &arr);
    if (err != 0) printf("Cannot create thread");
    switch(algo)
    {
        case HEAPSORT   : err = pthread_create(&t_heapsort, NULL, heapSort, &arr);
                          if (err != 0) printf("Cannot create thread");
                          err = pthread_join(t_heapsort, &tret);
                          if (err != 0) printf("Cannot join thread");
                          break;

        case MERGESORT  : err = pthread_create(&t_mergesort, NULL, mergeSort, &arr);
                          if (err != 0) printf("Cannot create thread");
                          err = pthread_join(t_mergesort, &tret);
                          if (err != 0) printf("Cannot join thread");
                          break;

        case BUBBLESORT : err = pthread_create(&t_bubblesort, NULL, bubbleSort, &arr);
                          if (err != 0) printf("Cannot create thread");
                          err = pthread_join(t_bubblesort, &tret);
                          if (err != 0) printf("Cannot join thread");
                          break;

        case RADIXSORT : err = pthread_create(&t_radixsort, NULL, radixSort, &arr);
                          if (err != 0) printf("Cannot create thread");
                          err = pthread_join(t_radixsort, &tret);
                          if (err != 0) printf("Cannot join thread");
                          break;
    }
    err = pthread_join(tdraw, &tret);
    if (err != 0) printf("Cannot join thread");
}
