#include <a2methods.h>

struct Closure
{
        A2Methods_T methods;
        A2 src;
};


struct Closure2 {
        A2Methods_T methods;
        unsigned denominator;
};

struct Closure3 {
    A2Methods_T methods;
    FILE *fp;

};