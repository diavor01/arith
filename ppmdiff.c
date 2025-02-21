#include <pnm.h>
#include <assert.h>
#include <a2methods.h>
#include "a2plain.h"
#include "math.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

typedef A2Methods_UArray2 A2;

// static bool check_pixel_difference 

unsigned min(unsigned a, unsigned b) 
{
        if (a < b) {
                return a;
        }
        return b;
}

// unsigned max(unsigned a, unsigned b) 
// {
//         if (a > b) {
//                 return a;
//         }
//         return b;
// } 

int main(int argc, char *argv[]) 
{
        // TODO: later use a2methods.c from solutions page

        assert(argc == 3);
        char *fileName1 = argv[1];
        char *fileName2 = argv[2];
        printf("file name 1 is %s\n", fileName1);
        printf("file name 2 is %s\n", fileName2);

        assert(!(strcmp(argv[1], "-") == 0 && strcmp(argv[2], "-") == 0));

        FILE *fp1;
        FILE *fp2;
        if (!(strcmp(fileName1, "-") == 0)) {
                fp1 = fopen(fileName1, "rb");
        } else {
                fp1 = stdin;
        }

        if (!(strcmp(fileName2, "-") == 0)) {
                fp2 = fopen(fileName2, "rb");
        } else {
                fp2 = stdin;
        }

        A2Methods_T methods = uarray2_methods_plain;

        Pnm_ppm image1 = Pnm_ppmread(fp1, methods);
        Pnm_ppm image2 = Pnm_ppmread(fp2, methods);

        int width1 = image1->width;
        int height1 = image1->height;
        A2 pixels1 = image1->pixels;

        int width2 = image2->width;
        int height2 = image2->height;
        A2 pixels2 = image2->pixels;


        // pixels1->pnm_rgb->red
        
        if (abs(width1 - width2) > 1 || abs(height1 - height2) > 1) {
                fprintf(stderr, "The images do not have the same size.");
                printf("1.0");
                exit(1);
        }

        int sum = 0;
        int denominator = 3;
        denominator *= min(height1, height2);
        denominator *= min(width1, width2);

        for (unsigned i = 0; i < min(width1, width2); i++) {
                for (unsigned j = 0; j < min(height1, height2); j++) {
                        Pnm_rgb elem1 = methods->at(pixels1, i, j);
                        unsigned red1 = elem1->red;
                        unsigned green1 = elem1->green;
                        unsigned blue1 = elem1->blue;

                        Pnm_rgb elem2 = methods->at(pixels2, i, j);
                        unsigned red2 = elem2->red;
                        unsigned green2 = elem2->green;
                        unsigned blue2 = elem2->blue;

                        sum += (red1 - red2) * (red1 - red2);
                        sum += (green1 - green2) * (green1 - green2);
                        sum += (blue1 - blue2) * (blue1 - blue2);
                }
        }

        float E = sqrt(sum / denominator);

        printf("%0.4f\n", E);

}