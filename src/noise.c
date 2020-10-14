#include "noise.h"
#include "math.h"
#include <stdlib.h>
#include <stdio.h>

//http://eastfarthing.com/blog/2015-04-21-noise/

// initialise
#define size_array 256
static int const size = size_array;
static int const mask = size_array - 1;

int perm[size_array];
float grads_x[size_array], grads_y[size_array];

void noise_init() {
    for (int index = 0; index < size; ++index) {
        int other = rand() % (index + 1);
        if (index > other)
            perm[index] = perm[other];
        perm[other] = index;
        grads_x[index] = cosf(2.0f * M_PI * index / size);
        grads_y[index] = sinf(2.0f * M_PI * index / size);

        printf("grads_x[index] = %f, ",grads_x[index]);
        printf("grads_y[index] = %f, ",grads_y[index]);
        printf("perm[index] = %f\n",perm[index]);
    }
}

float noise_f(float t) {
    t = fabsf(t);
    return t >= 1.0f ? 0.0f : 1.0f - (3.0f - 2.0f * t) * t * t;
}

float noise_surflet(float x, float y, float grad_x, float grad_y) {
    return noise_f(x) * noise_f(y) * (grad_x * x + grad_y * y);
}

float noise(float x, float y) {
    float result = 0.0f;
    int cell_x = floorf(x);
    int cell_y = floorf(y);
    for (int grid_y = cell_y; grid_y <= cell_y + 1; ++grid_y)
        for (int grid_x = cell_x; grid_x <= cell_x + 1; ++grid_x) {
            int hash = perm[(perm[grid_x & mask] + grid_y) & mask];
            result += noise_surflet(x - grid_x, y - grid_y, grads_x[hash], grads_y[hash]);
        }
    return result;
}