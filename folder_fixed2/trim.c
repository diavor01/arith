
#include <a2methods.h>
#include <pnm.h>
#include "closure.h"
#include "trim.h"
#include <assert.h>
#include <stdlib.h>
#include <stdio.h>

void trim_apply(int col, int row, A2 array, A2Methods_Object *elem, void *cl)
{
        //fprintf(stderr, "col is %i, row is %i in function trim_apply\n", col, row);

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