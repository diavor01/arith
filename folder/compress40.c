#include "compress40.h"
#include <a2methods.h>
#include "a2blocked.h"
#include <stdlib.h>
#include <stdio.h>
#include "assert.h"
#include <uarray.h>
#include "YPbPr.h"

// /comp/40/bin/images/flowers.ppm
// /comp/40/build/include/pnm.h
// /comp/40/build/include/a2methods.h

typedef A2Methods_UArray2 A2;

struct Closure
{
        A2Methods_T methods;
        A2 src;
};



float average(UArray_T values)
{
        assert(UArray_length(values) == 4);
        
        float sum = 0;

        for (int i = 0; i < 4; i++) {
                sum += *(float *)UArray_at(values, i);
        }

        return sum / 4;
}

struct Closure2 {
        A2Methods_T methods;
        UArray_T YPbPr_block_vals;
        unsigned denominator;
};

void create_new_closure_array(int row, int col, struct Closure2 *closure)
{
        if (row % 2 == 0 && col % 2 == 0)
        {
                fprintf(stderr, "Starting the block\n");
                UArray_T old = closure->YPbPr_block_vals;

                UArray_free(&old);
                
                UArray_T YPbPr_array = UArray_new(4, sizeof(PnmYPbPr));

                closure->YPbPr_block_vals = YPbPr_array;
        } 
}

void store_YPbPr_values(int row, int col, UArray_T array, PnmYPbPr new_val) 
{       
        int index;
        if (row % 2 == 0) {
                if (col % 2 == 0) {
                        index = 0;
                } else {
                        index = 1;
                }
        } else {
                if (col % 2 == 0) {
                        index = 2;
                } else {
                        index = 3;
                }       
        }

        PnmYPbPr curr = UArray_at(array, index);
        *curr = *new_val;
}

void apply(int col, int row, A2 array, A2Methods_Object *elem, void *cl) {

        struct Closure2 *closure = cl;
        create_new_closure_array(row, col, closure);
              
        PnmYPbPr curr = rgb_to_YPbPr(elem, closure->denominator);
        store_YPbPr_values(row, col, closure->YPbPr_block_vals, curr);
        

        if (row % 2 == 1 && col % 2 == 1)
        {
                fprintf(stderr, "Finished the block\n");

                for (int i = 0; i < 4; i++) {
                        UArray_T array = closure->YPbPr_block_vals;
                        printf("cell %i:");
                        printf("Y = %f, ", *((PnmYPbPr *)UArray_at(array, i)->Y));
                                
                                /*Y = %f, Pb = %f, Pr = %f", i, 
                                UArray_at(array, i)->Y, array[i]->Pb, array[i]->Pr);
                                *(float *)UArray_at(array, i);*/
        
                }
        }
        
}

void trim_apply(int col, int row, A2 array, A2Methods_Object *elem, void *cl)
{
        fprintf(stderr, "col is %i, row is %i\n", col, row);

        if (row % 2 == 1 && col % 2 == 1)
        {
                fprintf(stderr, "Finished the block\n");
        }

        assert(array != NULL);

        struct Closure *closure = cl;
        A2 original_array = closure->src;
        A2Methods_T methods = closure->methods;

        // assert(!(new_col < 0 || new_col >= methods->width(array) ||
        // new_row < 0 || new_row >= methods->height(array)));

        Pnm_rgb src = methods->at(original_array, col, row);
        assert(src != NULL);
        Pnm_rgb dest = elem;

        /*set pixel in new spot*/
        *dest = *src;
}

Pnm_ppm trim(Pnm_ppm image, A2Methods_T methods)
{
        A2 pixels = image->pixels;

        fprintf(stderr, "original height is %u\n", image->height);
        fprintf(stderr, "original width is %u\n", image->width);
        unsigned new_height = image->height;
        unsigned new_width = image->width;
        if (image->height % 2 == 1)
        {
                new_height -= 1;
        }
        if (image->width % 2 == 1)
        {
                new_width -= 1;
        }

        fprintf(stderr, "new heigth: %u\n", new_height);
        fprintf(stderr, "new width: %u\n", new_width);

        // const A2Methods_T methods = image->methods;
        A2 new_pixels = methods->new_with_blocksize(new_width, new_height, methods->size(pixels), 2);

        fprintf(stderr, "new pixels height is %i\n", methods->height(new_pixels));
        fprintf(stderr, "new pixels width is %i\n", methods->width(new_pixels));

        struct Closure cl = {methods, pixels};

        methods->map_block_major(new_pixels, trim_apply, &cl);

        Pnm_ppm trimmed = malloc(sizeof(*trimmed));
        trimmed->width = new_width;
        trimmed->height = new_height;
        trimmed->denominator = image->denominator;
        trimmed->pixels = new_pixels;
        trimmed->methods = methods;

        return trimmed;
}

/* reads PPM, writes compressed image */
extern void compress40(FILE *input)
{
        A2Methods_T methods = uarray2_methods_blocked;
        Pnm_ppm image = Pnm_ppmread(input, methods);

        fprintf(stderr, "hey\n");

        Pnm_ppm trimmed_image = trim(image, methods);

        fprintf(stderr, "trimmed image height is %i\n", trimmed_image->height);
        fprintf(stderr, "trimmed image width is %i\n", trimmed_image->width);

        // printf("new height is %u\n", trimmed_image->height);
        // printf("new width is %u\n", trimmed_image->width);

        fprintf(stderr, "Printing the image to stdout\n");
        Pnm_ppmwrite(stdout, trimmed_image);
}

/* reads compressed image, writes PPM */
extern void decompress40(FILE *input)
{
        (void)input;
}

int main(int argc, char *argv[])
{
        // FILE *fp = fopen(argv[1], "r");

        // compress40(fp);

        (void)argc;
        (void)argv;

        Pnm_rgb test = malloc(sizeof(*test));
        test->red = 78;
        test->green = 214;
        test->blue = 72;

        unsigned denominator = 255;

        PnmYPbPr val = rgb_to_YPbPr(test, denominator);

        printf("Y value: %f\n", val->Y);
        printf("Pb value: %f\n", val->Pb);
        printf("Pr value: %f\n", val->Pr);

        Pnm_rgb new_val = YPbPr_to_rgb(val, denominator);

        printf("R value: %u\n", new_val->red);
        printf("G value: %u\n", new_val->green);
        printf("B value: %u\n", new_val->blue);
        
}
