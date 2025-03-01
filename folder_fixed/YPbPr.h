#ifndef YPBPR
#define YPBPR

#include <pnm.h>

typedef struct pnmYPbPr
{
        float Y, Pb, Pr;
} *PnmYPbPr;

Pnm_rgb YPbPr_to_rgb(PnmYPbPr YPbPr, unsigned denominator);
PnmYPbPr rgb_to_YPbPr(Pnm_rgb RGB, unsigned denominator);

#endif
