#include "utilities.h"
#include <assert.h>
#include "stdio.h"
#include "stdlib.h"


float average(float v1, float v2, float v3, float v4)
{      
        return (v1 + v2 + v3 + v4) / 4;
}

void round_bcd(float *val)
{
    if (*val > 0.3) { *val = 0.3; }
    if (*val < -0.3) { *val = -0.3; }
}

void check_abcd_values(float *a, float *b, float *c, float *d)
{
        assert(*a >= 0 && *a <= 1);
        assert(*b >= -0.5 && *b <= 0.5);
        assert(*c >= -0.5 && *c <= 0.5);
        assert(*d >= -0.5 && *d <= 0.5);

        round_bcd(b);
        round_bcd(c);
        round_bcd(d);
}

void check_Y_value(float *Y)
{
        if (*Y < 0) {
                fprintf(stderr, "This values is negative: %f\n", *Y);
                *Y = 0;
        }
        if (*Y > 1) {
                fprintf(stderr, "This values is above 1: %f\n", *Y);
                *Y = 1;
        }
}

void print_binary(int64_t signed_n, uint64_t n, bool is_signed) {

        if (is_signed) { n = (uint64_t)signed_n; }

        uint64_t val = 1ULL << 63;
        for (long unsigned i = 0; i < sizeof(n) * 8; i++) {
                uint64_t curr_dig = val & n;
                curr_dig = curr_dig >> 63;
                fprintf(stderr, "%lu", curr_dig);
                n = n << 1ULL;
        }
        fprintf(stderr, "\n");
}

