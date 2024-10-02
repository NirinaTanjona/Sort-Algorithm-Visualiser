#include "raylib.h"
#include <stdio.h>

#define SAMPLE_LEN(xs) (sizeof(xs)/sizeof((xs)[0]))


int min(int x, int y) { return (x < y)? x : y; }

typedef struct QuickSortData
{
    int top;
    int high;
    int low;
    int stack[256];
} QuickSortData;

QuickSortDataInit(int top, int low, int high, QuickSortData *data)
{
    data->top = top;
    data->high = high;
    data->low = low;
    data->stack[++data->top] = data->low;
    data->stack[++data->top] = data->high;
}


void swap(size_t* xp, size_t* yp)
{
    size_t temp = *xp;
    *xp = *yp;
    *yp = temp;
}

void heapify(size_t arr[], int n)
{
    for (int i = 1; i < n; ++i) {
        if (arr[i] > arr[(i - 1) / 2]) {
            int j = i;

            while (arr[j] > arr[(j - 1) / 2]) {
                swap(&arr[j], &arr[(j - 1) / 2]);
                j = (j - 1) / 2;
            }
        }
    }
}

void heapSort(size_t arr[], int n)
{
    heapify(arr, n);

    // for (int i = n - 1; i > 0; --i) {

    //     swap(&arr[0], &arr[i]);

    //     int j = 0, index;

    //     do {
    //         index = (2 * j + 1);

    //         if (arr[index] < arr[index + 1] && index < (i - 1)) {
    //             ++index;
    //         }
    //         if (arr[j] < arr[index] && index < i) {
    //             swap(&arr[j], &arr[index]);
    //         }
    //         j = index;
    //     } while (index < i);
    // }
}


int partition(size_t arr[], int l, int h)
{
    size_t x = arr[h];
    int i = (l - 1);

    for (int j = l; j <= h - 1; ++j) {
        if (arr[j] <= x) {
            ++i;
            swap(&arr[i], &arr[j]);

        }
    }
    swap(&arr[i+1], &arr[h]);
    return (i+1);
}

void quickSort(size_t array[], QuickSortData *data)
{
    if (data->top >= 0) {
        data->high = data->stack[data->top--];
        data->low = data->stack[data->top--];

        int part = partition(array, data->low, data->high);

        if (part - 1 > data->low) {
            data->stack[++data->top] = data->low;
            data->stack[++data->top] = part - 1;
        }

        if (part + 1 < data->high) {
            data->stack[++data->top] = part + 1;
            data->stack[++data->top] = data->high;
        }
    }
}

void bubbleSort(size_t array[], int length)
{
    int swapped = false;
    for (int j = 0; j < length - 1; ++j) {
        if (array[j] > array[j + 1]) {
            swapped = true;
            break;
        }
    }

    if (swapped) {
        for (int i = 0; i < length - 1; ++i) {
            if (array[i] > array[i + 1]) {
                swap(&array[i], &array[i + 1]);
            }
        }
    }
}

void merge(size_t array[], int l, int m, int r)
{
    int i, j, k;
    int n1 = m - l + 1;
    int n2 = r - m;

    // Create temp arrays
    size_t L[n1], R[n2];

    // Copy data to temp arrays L[] and R[]
    for (i = 0; i < n1; ++i) {
        L[i] = array[l + i];
    }
    for (j = 0; j < n2; ++j) {
        R[j] = array[m + 1 + j];
    }

    // Merge the temp arrays back into arr[1..r]
    i = 0;
    j = 0;
    k = l;
    while (i < n1 && j < n2) {
        if (L[i] <= R[j]) {
            array[k] = L[i];
            ++i;
        }
        else {
            array[k] = R[j];
            ++j;
        }
        ++k;
    }

    // Copy the remaining elements of L[],
    // if there are any
    while (i < n1) {
        array[k] = L[i];
        ++i;
        ++k;
    }

    // Copy the remaining elements of R[],
    // if there are any
    while (j < n2) {
        array[k] = R[j];
        ++j;
        ++k;
    }
}

void mergeSort(size_t array[], int n)
{
    static int curr_size = 1;
    int left_start;

    int swapped = false;
    for (int j = 0; j < n - 1; ++j) {
        if (array[j] > array[j + 1]) {
            swapped = true;
            break;
        }
    }

    for (left_start = 0; left_start < n-1; left_start += 2*curr_size)
    {
        int mid = min(left_start + curr_size - 1, n-1);
        int right_end = min(left_start + 2*curr_size - 1, n-1);
        merge(array, left_start, mid, right_end);
    }

    if (curr_size <= n-1 && swapped) {
        curr_size = 2*curr_size;
    } else {
        curr_size = 1;
    }
}


int main(void)

{
    const int screenWidth = 800;
    const int screenHeight = 600;
    const int max_random_number = 100;
    const int sample_size = 200;
    bool bubbleSorting = true;
    bool mergeSorting = false;


    size_t samples[sample_size];
    for(int i = 0; i < sample_size; ++i) {
        samples[i] = GetRandomValue(1, 180);
    }


    InitWindow(screenWidth, screenHeight, "Raylib window");
    SetTargetFPS(10);

    int w = GetRenderWidth();
    int h = GetRenderHeight();

    int sample_count = SAMPLE_LEN(samples);

    // Quick Sort algorithm initial data
    QuickSortData data, *data_ptr;
    data_ptr = &data;
    QuickSortDataInit(-1, 0, sample_count-1, data_ptr);
    float cell_width = (float)w / sample_count;


    while(!WindowShouldClose())
    {
        BeginDrawing();

        for (int i = 0; i < sample_count; ++i) {
            float t = (float)samples[i] / max_random_number;
            DrawRectangle(i*cell_width, h - h/2*t, cell_width, h*t, BLUE);
        }

        // bubbleSort(samples, sample_count);

        // mergeSort(samples, sample_count);

        // quickSort(samples, data_ptr);

        heapSort(samples, sample_count);


        ClearBackground(CLITERAL(Color) {0x18, 0x18, 0x18, 0xFF});
        EndDrawing();
    }

    CloseWindow();

    return 0;
}
