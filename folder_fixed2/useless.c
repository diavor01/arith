// void store_YPbPr_values(int row, int col, UArray_T array, PnmYPbPr new_val) 
// {       

//         int index;
//         if (row % 2 == 0) {
//                 if (col % 2 == 0) {
//                         index = 0;
//                 } else {
//                         index = 1;
//                 }
//         } else {
//                 if (col % 2 == 0) {
//                         index = 2;
//                 } else {
//                         index = 3;
//                 }       
//         }


//         fprintf(stderr, "The index is %d in function store_YPbPr_values\n", index);
//         fprintf(stderr, "before - in store function, Y = %f\n", new_val->Y);
//         fprintf(stderr, "before - in store function, Pb = %f\n", new_val->Pb);
//         fprintf(stderr, "before - in store function, Pr = %f\n", new_val->Pr);
//         PnmYPbPr curr = UArray_at(array, index);
//         *curr = *new_val;
//         fprintf(stderr, "after - in store function, Y = %f\n", curr->Y);
//         fprintf(stderr, "after - in store function, Pb = %f\n", curr->Pb);
//         fprintf(stderr, "after - in store function, Pr = %f\n", curr->Pr);
// }

// void create_new_closure_array(int row, int col, struct Closure2 *closure)
// {
//         if (row % 2 == 0 && col % 2 == 0)
//         {
//                 fprintf(stderr, "Starting the block in function create new closure\n");
//                 UArray_T old = closure->YPbPr_block_vals;

//                 UArray_free(&old);
                
//                 UArray_T YPbPr_array = UArray_new(4, sizeof(PnmYPbPr));
//                 for (int i = 0; i < UArray_length(YPbPr_array); i++) {
//                         PnmYPbPr curr_struct = (PnmYPbPr)UArray_at(YPbPr_array, i);
//                         curr_struct->Y = 0.0;
//                         curr_struct->Pb = 0.0;
//                         curr_struct->Pr= 0.0;
//                         // fprintf(stderr, "before storing new values, Y = %f\n", curr_struct->Y);
//                         // fprintf(stderr, "before storing new values, Pb = %f\n", curr_struct->Pb);
//                         // fprintf(stderr, "before storing new values, Pr = %f\n", curr_struct->Pr);
//                 }

//                 closure->YPbPr_block_vals = YPbPr_array;

//                 //fprintf(stderr, "\n\n\n");
//         } 
// }

/*PnmYPbPr curr = rgb_to_YPbPr(elem, closure->denominator);
        store_YPbPr_values(row, col, closure->YPbPr_block_vals, curr);
        if (row % 2 == 0 && col % 2 == 0) {
                fprintf(stderr, "after updating cell 0: \n");
        } else if (row % 2 == 1 && col % 2 == 0) {
                fprintf(stderr, "after updating cell 2: \n");
        } else if (row % 2 == 0 && col % 2 == 1) {
                fprintf(stderr, "after updating cell 1: \n");
        } else {
                fprintf(stderr, "after updating cell 3: \n");
        }*/

        // for (int i = 0; i < 4; i++) {
        //         fprintf(stderr, "cell: %i\n", i);
        //         PnmYPbPr curr_struct = (PnmYPbPr)UArray_at(array, i);
        //         if (!curr_struct) {
        //                 fprintf(stderr, "It does not exist. i is: %d\n", i);
        //         } else {
        //                 fprintf(stderr, "Y = %f, ", curr_struct->Y);
        //                 fprintf(stderr, "Pb = %f, ", curr_struct->Pb);
        //                 fprintf(stderr, "Pr = %f, \n", curr_struct->Pr);
        //         }
                
        //         /*fprintf(stderr, "the denominator is: %u\n", closure->denominator);
        //         Pnm_rgb test = YPbPr_to_rgb(curr_struct, closure->denominator);
        //         fprintf(stderr, "R = %u, ", test->red);
        //         fprintf(stderr, "G = %u, ", test->green);
        //         fprintf(stderr, "B = %u, \n", test->blue);*/
        // }

        // printf("\n\n\n");

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


                        // TESTING
                // Pnm_rgb tl_test = YPbPr_to_rgb(tl_YPbPr, closure->denominator);
                // fprintf(stderr, "top left R = %u, ", tl_test->red);
                // fprintf(stderr, "top left G = %u, ", tl_test->green);
                // fprintf(stderr, "top left B = %u, \n", tl_test->blue);

                // Pnm_rgb bl_test = YPbPr_to_rgb(bl_YPbPr, closure->denominator);
                // fprintf(stderr, "bottom left R = %u, ", bl_test->red);
                // fprintf(stderr, "bottom left G = %u, ", bl_test->green);
                // fprintf(stderr, "bottom left B = %u, \n", bl_test->blue);

                // Pnm_rgb tr_test = YPbPr_to_rgb(tr_YPbPr, closure->denominator);
                // fprintf(stderr, "top right R = %u, ", tr_test->red);
                // fprintf(stderr, "top right G = %u, ", tr_test->green);
                // fprintf(stderr, "top right B = %u, \n", tr_test->blue);

                // Pnm_rgb br_test = YPbPr_to_rgb(br_YPbPr, closure->denominator);
                // fprintf(stderr, "bottom right R = %u, ", br_test->red);
                // fprintf(stderr, "bottom right G = %u, ", br_test->green);
                // fprintf(stderr, "bottom right B = %u, \n", br_test->blue);

                // fprintf(stderr, "\n\n\n");



                                /*uint64_t byte;
                uint64_t word = 0;
                int counter = 4;
                unsigned word_width = 8;

                while (counter > 0 && (byte = getc(input)) != (uint64_t)EOF) {
                        int64_t dummy = 0;
                        print_binary(dummy, byte, false);
                        fprintf(stderr, "\n");
                        word = Bitpack_newu(word, word_width, (counter) * word_width, byte);
                        word = Bitpack_newu(word, word_width, (counter) * word_width, byte);
                        word = Bitpack_newu(word, word_width, (counter) * word_width, byte);
                        word = Bitpack_newu(word, word_width, (counter) * word_width, byte);

                        if (counter == 0) {
                                print_binary(dummy, word, false);
                                fprintf(stderr, "\n\n");
                                
                                
                                counter = 3;
                                word = 0;
                        } else {
                                counter--;
                        }
                }*/