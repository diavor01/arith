#include "YPbPr.h"
#include "stdio.h"
#include "stdlib.h"
#include "assert.h"

PnmYPbPr rgb_to_YPbPr(Pnm_rgb RGB, unsigned denominator)
{
        float R = RGB->red;
        float G = RGB->green;
        float B = RGB->blue;

        printf("in rgb_toYPbPr, original red is %f\n", R);
        printf("in rgb_toYPbPr, original green is %f\n", G);
        printf("in rgb_toYPbPr, origginal blue is %f\n", B);

        /* Allocate memory for the struct */
        PnmYPbPr result = malloc(sizeof(*result));
        assert(result != NULL);

        /* Convert RGB to YPbPr using standard formulas */
        result->Y = ((0.299 * R) + (0.587 * G) + (0.114 * B)) / denominator;
        result->Pb = ((-0.168736 * R) - 0.331264 * G + 0.5 * B) / denominator;
        result->Pr = ((0.5 * R) - (0.418688 * G) - (0.081312 * B)) / denominator;

        
        printf("in rgb_toYPbPr, Y is %f\n", result->Y);
        printf("in rgb_toYPbPr, Pb is %f\n", result->Pb);
        printf("in rgb_toYPbPr, Pr is %f\n", result->Pr);

        if (result->Pr > 0.5) {
                printf("HEREEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEE1\n");
        }

        return result;
}

Pnm_rgb YPbPr_to_rgb(PnmYPbPr YPbPr, unsigned denominator)
{
        if (YPbPr->Pr > 0.5) {
                printf("HEREEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEE2\n");
        }

        float Y = YPbPr->Y;
        float Pb = YPbPr->Pb;
        float Pr = YPbPr->Pr;

        printf("original float Y is %f\n", Y);

        printf("original float Pb is %f\n", Pb);

        printf("original float Pr is %f\n", Pr);


        Pnm_rgb result = malloc(sizeof(*result));
        assert(result != NULL);
                
        float zero_val = 0;

        if ((1.0 * Y + 0.0 * Pb + 1.402 * Pr) < zero_val)
        {
                result->red = zero_val;
        } else {
                result->red = (1.0 * Y + 0.0 * Pb + 1.402 * Pr) * denominator;
        }

        if ((1.0 * Y - 0.344136 * Pb - 0.714136 * Pr) < zero_val) {
                result->green = zero_val;
        } else {
                result->green = (1.0 * Y - 0.344136 * Pb - 0.714136 * Pr) * denominator;
        }
        
        if ((1.0 * Y + 1.772 * Pb + 0.0 * Pr) < zero_val) {
                result->blue = zero_val;
        } else {
                result->blue = (1.0 * Y + 1.772 * Pb + 0.0 * Pr) * denominator;
        }    
                

        printf("in YPbPr_to_rgb, unsigned red is %u\n", result->red);

        printf("in YPbPr_to_rgb, usigned green is %u\n", result->green);

        printf("in YPbPr_to_rgb, unsigned blue is %u\n", result->blue);

        return result;
}