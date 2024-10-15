#include "algo.h"
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>

void Wait(double seconds)
{
    if (seconds < 0) return;
    struct timespec req = { 0 };
    time_t sec = seconds;
    long nsec = (seconds - sec)*1000000000L;
    req.tv_sec = sec;
    req.tv_nsec = nsec;

    while (nanosleep(&req, &req) == -1) continue;
}


void swap(size_t* xp, size_t* yp)
{
    size_t temp = *xp;
    *xp = *yp;
    *yp = temp;
}


int min(int x, int y) { return (x < y)? x : y; }


//----------------------------------------------------------------------------------
//                       HEAP SORT
//----------------------------------------------------------------------------------


void heapify(size_t arr[], int n)
{
    for (int i = 1; i < n; ++i)
        if (arr[i] > arr[(i - 1) / 2])
        {
            int j = i;
            while (arr[j] > arr[(j - 1) / 2])
            {
                swap(&arr[j], &arr[(j - 1) / 2]);
                Wait(0.1);
                j = (j - 1) / 2;
            }
        }
}

void* heapSort(void* arg)
{
    Array *data = arg;
    size_t *arr = data->items;
    int n = data->count;
    float sec = 0.1;
    heapify(arr, n);
    for (int i = n - 1; i > 0; --i)
    {

        swap(&arr[0], &arr[i]);
        Wait(sec);

        int j = 0, index;

        do {
            index = (2 * j + 1);

            if (arr[index] < arr[index + 1] && index < (i - 1))
                ++index;
            if (arr[j] < arr[index] && index < i)
            {
                swap(&arr[j], &arr[index]);
                Wait(sec);
            }
            j = index;
        } while (index < i);
    }
}


//----------------------------------------------------------------------------------
//                       BUBBLE SORT
//----------------------------------------------------------------------------------
void* bubbleSort(void* arg)
{
    Array *data = arg;
    size_t *arr = data->items;
    int n = data->count;
    bool swapped;
    for (int i = 0;  i < n - 1; ++i) {
        swapped = false;
        for (int j = 0; j < n - i - 1; ++j) {
            if (arr[j] > arr[j + 1]) {
                swap(&arr[j], &arr[j + 1]);
                Wait(0.1);
                swapped = true;
            }
        }
        if (swapped == false)
            break;
    }
}

//----------------------------------------------------------------------------------
//                       MERGE SORT
//----------------------------------------------------------------------------------

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
            Wait(0.1);
            ++i;
        }
        else {
            array[k] = R[j];
            Wait(0.1);
            ++j;
        }
        ++k;
    }

    // Copy the remaining elements of L[],
    // if there are any
    while (i < n1) {
        array[k] = L[i];
        Wait(0.1);
        ++i;
        ++k;
    }

    // Copy the remaining elements of R[],
    // if there are any
    while (j < n2) {
        array[k] = R[j];
        Wait(0.1);
        ++j;
        ++k;
    }
}


void* mergeSort(void* arg)
{
    Array *data = arg;
    size_t *arr = data->items;
    int n = data->count;
    int curr_size;
    int left_start;

    for (curr_size = 1; curr_size <= n - 1; curr_size = 2*curr_size)
        for (left_start = 0; left_start < n - 1; left_start += 2*curr_size)
        {
            int mid = min(left_start + curr_size - 1, n - 1);
            int right_end = min(left_start + 2*curr_size - 1, n - 1);

            merge(arr, left_start, mid, right_end);
        }
}


//----------------------------------------------------------------------------------
//                       RADIX SORT
//----------------------------------------------------------------------------------



int getMax(size_t arr[], int n)
{
    int mx = arr[0];
    for (int i = 1; i < n; ++i)
        if (arr[i] > mx)
            mx = arr[i];
    return mx;
}


void countSort(size_t arr[], int n, int exp)
{
    int output[n];
    int count[10] = {0};

    for (int i = 0; i < n; ++i)
        count[(arr[i] / exp) % 10]++;

    for (int i = 1; i < 10; ++i)
        count[i] += count[i - 1];

    for (int i = n - 1; i >= 0; --i) {
        output[count[(arr[i] / exp) % 10] - 1] = arr[i];
        count[(arr[i] / exp) % 10]--;
    }

    for (int i = 0; i < n; ++i) {
        arr[i] = output[i];
        Wait(0.5);
    }
}


void* radixSort(void* arg)
{
    Array *data = arg;
    size_t *arr = data->items;
    int n = data->count;

    int m = getMax(arr, n);

    for (int exp = 1; m / exp > 0; exp *= 10)
        countSort(arr, n, exp);
}
