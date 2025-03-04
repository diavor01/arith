#ifndef TRIM
#define TRIM

// #include "pnm.h"
// #include "closure.h"

void trim_apply(int col, int row, A2 array, A2Methods_Object *elem, void *cl);
Pnm_ppm trim(Pnm_ppm image, A2Methods_T methods);

#endif