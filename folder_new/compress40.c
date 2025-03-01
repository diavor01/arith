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
                fprintf(stderr, "Starting the block in function create new closure\n");
                UArray_T old = closure->YPbPr_block_vals;

                UArray_free(&old);
                
                UArray_T YPbPr_array = UArray_new(4, sizeof(PnmYPbPr));
                for (int i = 0; i < UArray_length(YPbPr_array); i++) {
                        PnmYPbPr curr_struct = (PnmYPbPr)UArray_at(YPbPr_array, i);
                        curr_struct->Y = 0.0;
                        curr_struct->Pb = 0.0;
                        curr_struct->Pr= 0.0;
                        fprintf(stderr, "before storing new values, Y = %f\n", curr_struct->Y);
                        fprintf(stderr, "before storing new values, Pb = %f\n", curr_struct->Pb);
                        fprintf(stderr, "before storing new values, Pr = %f\n", curr_struct->Pr);
                }

                closure->YPbPr_block_vals = YPbPr_array;

                fprintf(stderr, "\n\n\n");
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


        fprintf(stderr, "The index is %d in function store_YPbPr_values\n", index);
        fprintf(stderr, "before - in store function, Y = %f\n", new_val->Y);
        fprintf(stderr, "before - in store function, Pb = %f\n", new_val->Pb);
        fprintf(stderr, "before - in store function, Pr = %f\n", new_val->Pr);
        PnmYPbPr curr = UArray_at(array, index);
        *curr = *new_val;
        fprintf(stderr, "after - in store function, Y = %f\n", curr->Y);
        fprintf(stderr, "after - in store function, Pb = %f\n", curr->Pb);
        fprintf(stderr, "after - in store function, Pr = %f\n", curr->Pr);
}

void apply(int col, int row, A2 array, A2Methods_Object *elem, void *cl) {
        fprintf(stderr, "col is %i, row is %i in function apply\n", col, row);
        
        struct Closure2 *closure = cl;
        create_new_closure_array(row, col, closure);
              
        PnmYPbPr curr = rgb_to_YPbPr(elem, closure->denominator);
        store_YPbPr_values(row, col, closure->YPbPr_block_vals, curr);
        if (row % 2 == 0 && col % 2 == 0) {
                fprintf(stderr, "after updating cell 0: \n");
        } else if (row % 2 == 1 && col % 2 == 0) {
                fprintf(stderr, "after updating cell 2: \n");
        } else if (row % 2 == 0 && col % 2 == 1) {
                fprintf(stderr, "after updating cell 1: \n");
        } else {
                fprintf(stderr, "after updating cell 3: \n");
        }

        for (int i = 0; i < 4; i++) {
                fprintf(stderr, "cell: %i\n", i);
                PnmYPbPr curr_struct = (PnmYPbPr)UArray_at(array, i);
                if (!curr_struct) {
                        fprintf(stderr, "It does not exist. i is: %d\n", i);
                } else {
                        fprintf(stderr, "Y = %f, ", curr_struct->Y);
                        fprintf(stderr, "Pb = %f, ", curr_struct->Pb);
                        fprintf(stderr, "Pr = %f, \n", curr_struct->Pr);
                }
                
                /*fprintf(stderr, "the denominator is: %u\n", closure->denominator);
                Pnm_rgb test = YPbPr_to_rgb(curr_struct, closure->denominator);
                fprintf(stderr, "R = %u, ", test->red);
                fprintf(stderr, "G = %u, ", test->green);
                fprintf(stderr, "B = %u, \n", test->blue);*/
        }

        printf("\n\n\n");

        /*if (row % 2 == 1 && col % 2 == 1)
        {
                fprintf(stderr, "Finished the block in function apply\n");

                UArray_T array = closure->YPbPr_block_vals;
                fprintf(stderr, "length of array is %i\n", UArray_length(array));

                fprintf(stderr, "Started printing the elements in the block in function apply\n");
                for (int i = 0; i < 4; i++) {
                        fprintf(stderr, "cell: %i\n", i);
                        PnmYPbPr curr_struct = (PnmYPbPr)UArray_at(array, i);
                        if (!curr_struct) {
                                fprintf(stderr, "It does not exist. i is: %d\n", i);
                        } else {
                                fprintf(stderr, "Y = %f, ", curr_struct->Y);
                                fprintf(stderr, "Pb = %f, ", curr_struct->Pb);
                                fprintf(stderr, "Pr = %f, \n", curr_struct->Pr);
                        }
                        
                        fprintf(stderr, "the denominator is: %u\n", closure->denominator);
                        Pnm_rgb test = YPbPr_to_rgb(curr_struct, closure->denominator);
                        fprintf(stderr, "R = %u, ", test->red);
                        fprintf(stderr, "G = %u, ", test->green);
                        fprintf(stderr, "B = %u, \n", test->blue);
                }
        }*/
        
        
        (void)array;
}



void trim_apply(int col, int row, A2 array, A2Methods_Object *elem, void *cl)
{
        fprintf(stderr, "col is %i, row is %i in function trim_apply\n", col, row);

        /*if (row % 2 == 1 && col % 2 == 1)
        {
                fprintf(stderr, "Finished the block in \n");
        }*/

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

        fprintf(stderr, "\nThe image sucessfully trimmed\n");

        //Pnm_ppm trimmed_image = image;

        //fprintf(stderr, "trimmed image height is %i\n", trimmed_image->height);
        //fprintf(stderr, "trimmed image width is %i\n", trimmed_image->width);

        UArray_T YPbPr_array = UArray_new(4, sizeof(PnmYPbPr));
        //struct Closure2 *cl = malloc(sizeof(*cl));
        
        struct Closure2 cl = {methods, YPbPr_array, trimmed_image->denominator};
        
        methods->map_block_major(trimmed_image->pixels, apply, &cl);

        // printf("new height is %u\n", trimmed_image->height);
        // printf("new width is %u\n", trimmed_image->width);

        fprintf(stderr, "Printing the image\n");
        FILE *output_fp = fopen("test.ppm", "wb");
        Pnm_ppmwrite(output_fp, trimmed_image);
}

/* reads compressed image, writes PPM */
extern void decompress40(FILE *input)
{
        (void)input;
}

int main(int argc, char *argv[])
{
        // (void)argv;
        (void)argc;

        FILE *fp = fopen(argv[1], "r");

        compress40(fp);
        /*Pnm_rgb test = malloc(sizeof(*test));
        test->red = 0;
        test->green = 15;
        test->blue = 7;
        rgb_to_YPbPr(test, 15);*/

        /*PnmYPbPr val = rgb_to_YPbPr(test, 15);
        printf("Y value: %f\n", val->Y);
        printf("Pb value: %f\n", val->Pb);
        printf("Pr value: %f\n", val->Pr);*/

        /*PnmYPbPr val = malloc(sizeof(*val));
        val->Y = 0;
        val->Pb = 0.331264;
        val->Pr = 0;
        Pnm_rgb new_val = YPbPr_to_rgb(val, 15);
        printf("R value: %u\n", new_val->red);
        printf("G value: %u\n", new_val->green);
        printf("B value: %u\n", new_val->blue);*/

        // TODO: round to nearest int! to avoid unnecessary loss 
        // Pnm_rgb test = malloc(sizeof(*test));
        // test->red = 78;
        // test->green = 214;
        // test->blue = 72;

        // unsigned denominator = 255;

        // PnmYPbPr val = rgb_to_YPbPr(test, denominator);

        // printf("Y value: %f\n", val->Y);
        // printf("Pb value: %f\n", val->Pb);
        // printf("Pr value: %f\n", val->Pr);

        // Pnm_rgb new_val = YPbPr_to_rgb(val, denominator);

        // printf("R value: %u\n", new_val->red);
        // printf("G value: %u\n", new_val->green);
        // printf("B value: %u\n", new_val->blue);
        
}
