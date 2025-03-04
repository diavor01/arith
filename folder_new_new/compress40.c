#include "compress40.h"
#include <a2methods.h>
#include "a2blocked.h"
#include <stdlib.h>
#include <stdio.h>
#include "assert.h"
#include <uarray.h>
#include "YPbPr.h"
#include "utilities.h"
#include "arith40.h"
#include "math.h"
#include "bitpack.h"
#include "closure.h"
#include "trim.h"

// /comp/40/bin/images/flowers.ppm
// /comp/40/build/include/pnm.h
// /comp/40/build/include/a2methods.h

#define DENOMINATOR 255


// float* (float a, float b, float c, float d)
// {
//         float* list = malloc(4 * sizeof(float));
// }


UArray_T quantize_abcd(float *a, float *b, float *c, float *d) {
        UArray_T vector = UArray_new(4, 8);

        fprintf(stderr, "original a is %f\n", *a);
        fprintf(stderr, "original b is %f\n", *b);
        fprintf(stderr, "original c is %f\n", *c);
        fprintf(stderr, "original d is %f\n\n", *d);

        fprintf(stderr, "float rounded a is %f\n", round(*a * 511));

        uint64_t *a_spot = (uint64_t *)UArray_at(vector, 0);
        fprintf(stderr, "rounded a is %lu\n", (uint64_t )round(*a * 511));
        *a_spot = round(*a * 511);

        fprintf(stderr, "float rounded b is %f\n", round(*b * 50));

        int64_t *b_spot = (int64_t *)UArray_at(vector, 1);
        fprintf(stderr, "rounded b is %li\n", (int64_t )round(*b * 50));
        *b_spot = round(*b * 50);

        fprintf(stderr, "float rounded c is %f\n", round(*c * 50));

        int64_t *c_spot = (int64_t *)UArray_at(vector, 2);
        fprintf(stderr, "rounded c is %li\n", (int64_t )round(*c * 50));
        *c_spot = round(*c * 50);

        fprintf(stderr, "float rounded d is %f\n", round(*d * 50));

        int64_t *d_spot = (int64_t *)UArray_at(vector, 3);
        fprintf(stderr, "rounded d is %li\n", (int64_t )round(*d * 50));
        *d_spot = round(*d * 50);

        return vector;
}


UArray_T DCT(float Y1, float Y2, float Y3, float Y4)
{
        float a = (Y4 + Y3 + Y2 + Y1) / 4.0;
        float b = (Y4 + Y3 - Y2 - Y1) / 4.0;
        float c = (Y4 - Y3 + Y2 - Y1) / 4.0;
        float d = (Y4 - Y3 - Y2 + Y1) / 4.0;
        
        fprintf(stderr, "a: %f\n", a);
        fprintf(stderr, "b: %f\n", b);
        fprintf(stderr, "c: %f\n", c);
        fprintf(stderr, "d: %f\n", d);

        check_abcd_values(&a, &b, &c, &d);
        UArray_T quantized_abcd = quantize_abcd(&a, &b, &c, &d);

        return quantized_abcd;
}

uint64_t pack_word(uint64_t a, int64_t b, int64_t c, int64_t d,
                   uint64_t Pb, uint64_t Pr) {
        uint64_t word = 0;
        word = Bitpack_newu(word, 4, 0, Pr);
        word = Bitpack_newu(word, 4, 4, Pb);
        word = Bitpack_news(word, 5, 8, d);
        word = Bitpack_news(word, 5, 13, c);
        word = Bitpack_news(word, 5, 18, b);
        word = Bitpack_newu(word, 9, 23, a);

        return word;
}

void print_word_by_byte(uint64_t word) {
        unsigned width = 8;

        for (int lsb = 24; lsb >= 0; lsb -= 8) {
                uint8_t byte = (uint8_t)Bitpack_getu(word, width, (unsigned)lsb);
                putchar(byte);
        }
}

void apply(int col, int row, A2 array, A2Methods_Object *elem, void *cl) {
        //fprintf(stderr, "col is %i, row is %i in function apply\n", col, row);
        
        if (row % 2 == 1 && col % 2 == 1) {

                struct Closure2 *closure = cl;
                // create_new_closure_array(row, col, closure);

                A2Methods_T methods = closure->methods;

                /* calculate Y/Pb/Pr for top left */
                Pnm_rgb tl_rgb = (Pnm_rgb)methods->at(array, col - 1, row - 1);
                PnmYPbPr tl_YPbPr = rgb_to_YPbPr(tl_rgb, closure->denominator);

                /* calculate Y/Pb/Pr for bottom left */
                Pnm_rgb bl_rgb = (Pnm_rgb)methods->at(array, col - 1, row);
                PnmYPbPr bl_YPbPr = rgb_to_YPbPr(bl_rgb, closure->denominator);

                /* calculate Y/Pb/Pr for top right */
                Pnm_rgb tr_rgb = (Pnm_rgb)methods->at(array, col, row - 1);
                PnmYPbPr tr_YPbPr = rgb_to_YPbPr(tr_rgb, closure->denominator);

                /* calculate Y/Pb/Pr for bottom right */
                PnmYPbPr br_YPbPr = rgb_to_YPbPr(elem, closure->denominator);

                float avgPb = average(tl_YPbPr->Pb, bl_YPbPr->Pb, 
                                      tr_YPbPr->Pb, br_YPbPr->Pb);
                float avgPr = average(tl_YPbPr->Pr, bl_YPbPr->Pr, 
                                      tr_YPbPr->Pr, br_YPbPr->Pr);


                fprintf(stderr, "Pb values: %f, %f, %f, %f\n", 
                        tl_YPbPr->Pb, bl_YPbPr->Pb, tr_YPbPr->Pb, br_YPbPr->Pb);
                fprintf(stderr, "avgPb: %f\n", avgPb);
                
                fprintf(stderr, "Pr values: %f, %f, %f, %f\n", 
                        tl_YPbPr->Pr, bl_YPbPr->Pr, tr_YPbPr->Pr, br_YPbPr->Pr);
                fprintf(stderr, "avgPr: %f\n", avgPr);

                uint64_t quantized_Pb = Arith40_index_of_chroma(avgPb);
                uint64_t quantized_Pr = Arith40_index_of_chroma(avgPr);

                fprintf(stderr, "quantized Pb is %lu\n", quantized_Pb);
                fprintf(stderr, "quantized Pr is %lu\n", quantized_Pr);

                fprintf(stderr, "Y1 is %f\n", tl_YPbPr->Y);
                fprintf(stderr, "Y2 is %f\n", tr_YPbPr->Y);
                fprintf(stderr, "Y3 is %f\n", bl_YPbPr->Y);
                fprintf(stderr, "Y4 is %f\n", br_YPbPr->Y);


                UArray_T quantized_abcd = DCT(tl_YPbPr->Y, tr_YPbPr->Y, 
                                              bl_YPbPr->Y, br_YPbPr->Y);

                

                fprintf(stderr, "Printing abcd\n");
                for (int i = 0; i < 4; i++) {
                        if (i == 0) {
                                fprintf(stderr, "%lu ", *(uint64_t *)UArray_at(quantized_abcd, i));
                        } else {
                                fprintf(stderr, "%li ", *(int64_t *)UArray_at(quantized_abcd, i));
                        }
                }



                fprintf(stderr, "\n");

                uint64_t word = pack_word(*(uint64_t*)UArray_at(quantized_abcd, 0), 
                                         *(int64_t*)UArray_at(quantized_abcd, 1), 
                                         *(int64_t*)UArray_at(quantized_abcd, 2), 
                                         *(int64_t*)UArray_at(quantized_abcd, 3), 
                                              quantized_Pb, quantized_Pr);
                //int64_t dummy = 0;
                fprintf(stderr, "\n");
                //print_binary(dummy, word, false);
                print_word_by_byte(word);

                fprintf(stderr, "\n\n");
        }
              
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

        //UArray_T YPbPr_array = UArray_new(4, sizeof(PnmYPbPr));
        struct Closure2 cl = {methods, trimmed_image->denominator};
        
        printf("COMP40 Compressed image format 2\n%u %u\n", 
                                                        trimmed_image->width, 
                                                        trimmed_image->height);
        methods->map_block_major(trimmed_image->pixels, apply, &cl);

        // printf("new height is %u\n", trimmed_image->height);
        // printf("new width is %u\n", trimmed_image->width);

        fprintf(stderr, "Printing the image\n");
        // FILE *output_fp = fopen("test.ppm", "wb");
        // Pnm_ppmwrite(output_fp, trimmed_image);
}

UArray_T reverse_DCT(uint64_t a, int64_t b, int64_t c, int64_t d)
{       
        
        // fprintf(stderr, "original a: %lu\n", a);
        // fprintf(stderr, "original b: %li\n", b);
        // fprintf(stderr, "original c: %li\n", c);
        // fprintf(stderr, "original d: %li\n", d);
        
        float float_a = (float)a / 511;
        float float_b = (float)b / 50;
        float float_c = (float)c / 50;
        float float_d = (float)d / 50;

        // fprintf(stderr, "float a is %f\n", float_a);
        // fprintf(stderr, "float b is %f\n", float_b);
        // fprintf(stderr, "float c is %f\n", float_c);
        // fprintf(stderr, "float a is %f\n", float_d);

        float Y1 = float_a - float_b - float_c + float_d; 
        float Y2 = float_a - float_b + float_c - float_d; 
        float Y3 = float_a + float_b - float_c - float_d;
        float Y4 = float_a + float_b + float_c + float_d;

        check_Y_value(&Y1);
        check_Y_value(&Y2);
        check_Y_value(&Y3);
        check_Y_value(&Y4);

        UArray_T vector = UArray_new(4, 8);

        float *Y1_spot = (float *)UArray_at(vector, 0);
        *Y1_spot = Y1;
        // fprintf(stderr, "Y1 is %f\n", Y1);

        float *Y2_spot = (float *)UArray_at(vector, 1);
        *Y2_spot = Y2;
        // fprintf(stderr, "Y2 is %f\n", *Y2_spot);

        float *Y3_spot = (float *)UArray_at(vector, 2);
        *Y3_spot = Y3;
        // fprintf(stderr, "Y3 is %f\n", *Y3_spot);

        float *Y4_spot = (float *)UArray_at(vector, 3);
        *Y4_spot = Y4;
        // fprintf(stderr, "Y4 is %f\n", *Y4_spot);

        return vector;
}

PnmYPbPr create_YPbPr(float Y, float Pb, float Pr) {
        PnmYPbPr object = malloc(sizeof(*object));
        object->Y = Y;
        object->Pb = Pb;
        object->Pr = Pr;

        return object;
}

void decompress_apply(int col, int row, A2 array, A2Methods_Object *elem, void *cl) {
        fprintf(stderr, "col is %i, row is %i\n", col, row);
        if (row % 2 == 1 && col % 2 == 1) {
                fprintf(stderr, "End of block\n");
                struct Closure3 *closure = cl;
                A2Methods_T methods = closure->methods;
                FILE *input = closure->fp;

                uint64_t word = 0;
                int word_width = 8;
                int counter = 3;
                uint64_t byte;
                
                while (counter >= 0 && (byte = getc(input)) != (uint64_t)EOF) {
                        word = Bitpack_newu(word, word_width, counter * word_width, byte);
                        counter--;
                }

                assert(byte != (uint64_t)EOF);

                //int64_t dummy = 0;
                //print_binary(dummy, word, false);
                //printf("\n\n");

                uint64_t a = Bitpack_getu(word, 9, 23);
                int64_t b = Bitpack_gets(word, 5, 18);
                int64_t c = Bitpack_gets(word, 5, 13);
                int64_t d = Bitpack_gets(word, 5, 8);
                uint64_t Pb = Bitpack_getu(word, 4, 4);
                uint64_t Pr = Bitpack_getu(word, 4, 0);

                // fprintf(stderr, "a: %lu, b: %ld, c: %ld, d: %ld, Pb: %lu, Pr: %lu\n",
                        // a, b, c, d, Pb, Pr);
                
                float float_Pr = Arith40_chroma_of_index(Pr);
                float float_Pb = Arith40_chroma_of_index(Pb);

                // fprintf(stderr, "float Pr is %f\n", float_Pr);
                // fprintf(stderr, "float Pb is %f\n", float_Pb);

                UArray_T Y_values = reverse_DCT(a, b, c, d);
                
                PnmYPbPr tl_YPbPr = create_YPbPr(*(float*)UArray_at(Y_values, 0), float_Pb, float_Pr);
                PnmYPbPr tr_YPbPr = create_YPbPr(*(float*)UArray_at(Y_values, 1), float_Pb, float_Pr);
                PnmYPbPr bl_YPbPr = create_YPbPr(*(float*)UArray_at(Y_values, 2), float_Pb, float_Pr);
                PnmYPbPr br_YPbPr = create_YPbPr(*(float*)UArray_at(Y_values, 3), float_Pb, float_Pr);

                Pnm_rgb tl_rgb = YPbPr_to_rgb(tl_YPbPr, DENOMINATOR);
                Pnm_rgb tr_rgb = YPbPr_to_rgb(tr_YPbPr, DENOMINATOR);
                Pnm_rgb bl_rgb = YPbPr_to_rgb(bl_YPbPr, DENOMINATOR);
                Pnm_rgb br_rgb = YPbPr_to_rgb(br_YPbPr, DENOMINATOR);

                // fprintf(stderr, "top left R value: %u\n", tl_rgb->red);
                // fprintf(stderr, "top left G value: %u\n", tl_rgb->green);
                // fprintf(stderr, "top left B value: %u\n\n", tl_rgb->blue);

                // fprintf(stderr, "top right R value: %u\n", tr_rgb->red);
                // fprintf(stderr, "top right G value: %u\n", tr_rgb->green);
                // fprintf(stderr, "top right B value: %u\n\n", tr_rgb->blue);

                // fprintf(stderr, "bottom left R value: %u\n", bl_rgb->red);
                // fprintf(stderr, "bottom left G value: %u\n", bl_rgb->green);
                // fprintf(stderr, "bottom left B value: %u\n\n", bl_rgb->blue);

                // fprintf(stderr, "bottom right R value: %u\n", br_rgb->red);
                // fprintf(stderr, "bottom right G value: %u\n", br_rgb->green);
                // fprintf(stderr, "bottom right B value: %u\n\n", br_rgb->blue);

                /* update RGB for top left */
                Pnm_rgb tl_rgb_spot = (Pnm_rgb)methods->at(array, col - 1, row - 1);
                *tl_rgb_spot = *tl_rgb;

                /* update RGB for bottom left */
                Pnm_rgb bl_rgb_spot = (Pnm_rgb)methods->at(array, col - 1, row);
                *bl_rgb_spot = *bl_rgb;

                /* update RGB for top right */
                Pnm_rgb tr_rgb_spot = (Pnm_rgb)methods->at(array, col, row - 1);
                *tr_rgb_spot = *tr_rgb;

                /* update RGB for bottom right */
                Pnm_rgb br_rgb_spot = (Pnm_rgb)methods->at(array, col, row);
                *br_rgb_spot = *br_rgb;

                // fprintf(stderr, "spot: top left R value: %u\n", tl_rgb_spot->red);
                // fprintf(stderr, "spot: top left G value: %u\n", tl_rgb_spot->green);
                // fprintf(stderr, "spot: top left B value: %u\n\n", tl_rgb_spot->blue);

                // fprintf(stderr, "spot: top right R value: %u\n", tr_rgb_spot->red);
                // fprintf(stderr, "spot: top right G value: %u\n", tr_rgb_spot->green);
                // fprintf(stderr, "spot: top right B value: %u\n\n", tr_rgb_spot->blue);

                // fprintf(stderr, "spot: bottom left R value: %u\n", bl_rgb_spot->red);
                // fprintf(stderr, "spot: bottom left G value: %u\n", bl_rgb_spot->green);
                // fprintf(stderr, "spot: bottom left B value: %u\n\n", bl_rgb_spot->blue);

                // fprintf(stderr, "spot: bottom right R value: %u\n", br_rgb_spot->red);
                // fprintf(stderr, "spot: bottom right G value: %u\n", br_rgb_spot->green);
                // fprintf(stderr, "spot: bottom right B value: %u\n\n", br_rgb_spot->blue);
        }
        (void)elem;
}

/* reads compressed image, writes PPM */
extern void decompress40(FILE *input)
{
        // uint64_t c;
        unsigned height, width;
        int read = fscanf(input, "COMP40 Compressed image format 2\n%u %u", &width, &height);
        assert(read == 2);
        int c = getc(input);
        assert(c == '\n');

        A2Methods_T methods = uarray2_methods_blocked;
        A2 pixels = methods->new_with_blocksize(width, height, sizeof(Pnm_rgb), 2);

        struct Closure3 cl = {methods, input};

        methods->map_block_major(pixels, decompress_apply, &cl);

        Pnm_ppm pixmap = malloc(sizeof(*pixmap));
        pixmap->width = width;
        pixmap->height = height;
        pixmap->denominator = DENOMINATOR;
        pixmap->pixels = pixels;
        pixmap->methods = methods;

        Pnm_ppmwrite(stdout, pixmap);       
}

int main(int argc, char *argv[])
{
        // (void)argv;
        (void)argc;

        FILE *fp = fopen(argv[1], "rb");
        if (!fp) {
                perror("Error opening file");
                return 1;
        }
        //compress40(fp);
        decompress40(fp);
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