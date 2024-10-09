#include "raylib.h"
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#define                                                DEFAULT_MIN 1
#define                                              DEFAULT_MAX 100
#define                                              DEFAULT_SIZE 100


void swap(size_t* xp, size_t* yp)
{
    size_t temp = *xp;
    *xp = *yp;
    *yp = temp;
}


typedef struct
{
    size_t *items;
    size_t count;
    size_t capacity;
} Array;


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
            static int i = 1;

            for (size_t i = 0; i < arr->count; ++i) {
                float t = (float)arr->items[i] / DEFAULT_MAX;
                DrawRectangle(i*cell_width, h - h/1.2*t, cell_width-2, h*t, BLUE);
            }
            ClearBackground(CLITERAL(Color) {0x18, 0x18, 0x18, 0xFF});
            i += 10;
        EndDrawing();
    }
    CloseWindow();
}


void heapify(size_t arr[], int n)
{
    for (int i = 1; i < n; ++i)
        if (arr[i] > arr[(i - 1) / 2])
        {
            int j = i;
            while (arr[j] > arr[(j - 1) / 2])
            {
                swap(&arr[j], &arr[(j - 1) / 2]);
                WaitTime(0.2);
                j = (j - 1) / 2;
            }
        }
}

void* heapSort(void* arg)
{
    Array *data = arg;
    size_t *arr = data->items;
    int n = data->count;
    float sec = 0.2;
    heapify(arr, n);
    for (int i = n - 1; i > 0; --i)
    {

        swap(&arr[0], &arr[i]);
        WaitTime(sec);

        int j = 0, index;

        do {
            index = (2 * j + 1);

            if (arr[index] < arr[index + 1] && index < (i - 1))
                ++index;
            if (arr[j] < arr[index] && index < i)
            {
                swap(&arr[j], &arr[index]);
                WaitTime(sec);
            }
            j = index;
        } while (index < i);
    }
}


int main(void)
{
    int err;
    pthread_t heapsort, tdraw;
    void *tret;

    Array arr = {0};
    init(&arr);

    err = pthread_create(&heapsort, NULL, heapSort, &arr);
    if (err != 0) printf("Cannot create thread");

    err = pthread_create(&tdraw, NULL, draw, &arr);
    if (err != 0) printf("Cannot create thread");

    err = pthread_join(heapsort, &tret);
    if (err != 0) printf("Cannot join thread");

    err = pthread_join(tdraw, &tret);
    if (err != 0) printf("Cannot join thread");

    return 0;
}
