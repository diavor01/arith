#include "YPbPr.h"
#include "stdio.h"
#include "stdlib.h"
#include "assert.h"

PnmYPbPr rgb_to_YPbPr(Pnm_rgb RGB, unsigned denominator)
{
        unsigned R = RGB->red;
        unsigned G = RGB->green;
        unsigned B = RGB->blue;

        /* Allocate memory for the struct */
        PnmYPbPr result = malloc(sizeof(*result));
        assert(result != NULL);

        /* Convert RGB to YPbPr using standard formulas */
        result->Y = (0.299 * R + 0.587 * G + 0.114 * B) / denominator;
        result->Pb = (-0.168736 * R - 0.331264 * G + 0.5 * B) / denominator;
        result->Pr = (0.5 * R - 0.418688 * G - 0.081312 * B) / denominator;

        return result;
}

Pnm_rgb YPbPr_to_rgb(PnmYPbPr YPbPr, unsigned denominator)
{
        float Y = YPbPr->Y;
        float Pb = YPbPr->Pb;
        float Pr = YPbPr->Pr;

        //TODO: Maybe we should round it up. Ask TA
        printf("float red is %f\n", 1.0 * Y + 0.0 * Pb + 1.402 * Pr);

        printf("float green is %f\n", 1.0 * Y - 0.344136 * Pb - 0.714136 * Pr);

        printf("float blue is %f\n", 1.0 * Y + 1.772 * Pb + 0.0 * Pr);


        Pnm_rgb result = malloc(sizeof(*result));
        assert(result != NULL);
                
        result->red = (1.0 * Y + 0.0 * Pb + 1.402 * Pr) * denominator;
        result->green = (1.0 * Y - 0.344136 * Pb - 0.714136 * Pr) * denominator;
        result->blue = (1.0 * Y + 1.772 * Pb + 0.0 * Pr) *denominator;

        return result;
}