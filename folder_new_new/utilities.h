#ifndef UTILITIES
#define UTILITIES
#include <stdbool.h>
#include <stdint.h>


float average(float v1, float v2, float v3, float v4);
void check_abcd_values(float *a, float *b, float *c, float *d);
void print_binary(int64_t signed_n, uint64_t n, bool is_signed);
void check_Y_value(float *Y);

#endif
