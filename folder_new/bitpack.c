#include "bitpack.h"
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>

#include "except.h"
Except_T Bitpack_Overflow = { "Overflow packing bits" };

void print_binary(int64_t signed_n, uint64_t n, bool is_signed) {

        if (is_signed) { n = (uint64_t)signed_n; }

        uint64_t val = 1ULL << 63;
        for (long unsigned i = 0; i < sizeof(n) * 8; i++) {
                uint64_t curr_dig = val & n;
                curr_dig = curr_dig >> 63;
                printf("%lu", curr_dig);
                n = n << 1ULL;
        }
        printf("\n");
}


bool Bitpack_fitsu(uint64_t n, unsigned width)
{
        uint64_t val = 1ULL << width;
        return (n < val);
}

bool Bitpack_fitss(int64_t n, unsigned width)
{
        int64_t val = 1ULL << (width - 1);
        return ((0 - val) <= n && n < val);
}


uint64_t Bitpack_getu(uint64_t word, unsigned width, unsigned lsb)
{
        assert(width <= 64);
        assert(width + lsb <= 64);

        /* create mask */
        uint64_t mask = 1ULL << width;
        mask = mask - 1; 
        mask = mask << lsb;

        /* apply mask */
        word = word & mask;

        /* shift back */
        word = word >> lsb;

        return word;
}


int64_t Bitpack_gets(uint64_t word, unsigned width, unsigned lsb)
{
        assert(width <= 64);
        assert(width + lsb <= 64);
        
        uint64_t val = Bitpack_getu(word, width, lsb);
        uint64_t msb = 1ULL << (width - 1);

        // printf("%lu\n", val);
        // print_binary(val);

        // printf("%lu\n", msb);
        // print_binary(msb);

        /* positive case */
        if ((val & msb) == 0) {
                return (int64_t)val;
        } /* negative case */
        else {
                msb = msb << 1;
                // printf("New msb\n");
                // print_binary(msb);

                //printf("%lu\n", msb);
                return val - msb;
        }
}


uint64_t Bitpack_newu(uint64_t word, unsigned width, unsigned lsb, uint64_t value)
{       
        
        assert(width <= 64);
        assert(width + lsb <= 64);
        if (!Bitpack_fitsu(value, width)) {
                RAISE(Bitpack_Overflow);
        }
        
        uint64_t new_value = value << lsb;

        uint64_t mask = 1ULL << width;
        mask = mask - 1; 
        mask = mask << lsb;
        mask = ~mask;
        
        return ((word & mask) | new_value);
}


uint64_t Bitpack_news(uint64_t word, unsigned width, unsigned lsb,  int64_t value)
{
        assert(width <= 64);
        assert(width + lsb <= 64);
        if (!Bitpack_fitss(value, width)) {
                RAISE(Bitpack_Overflow);
        }

        int64_t mask = 1ULL << width;
        mask = mask - 1; 

        uint64_t new_value = (value & mask);

        // printf("This is new_value:\n");
        // print_binary(value, new_value, false);

        mask = mask << lsb;
        mask = ~mask;
        
        return ((word & mask) | new_value << lsb);
        
}

extern Except_T Bitpack_Overflow;

int main()
{
        // uint64_t n = 111;
        // print_binary(n);
        // printf("%lu\n", Bitpack_getu(10, 6, 2));

        int64_t dummy1 = 1;
        //uint64_t dummy2 = 1;

        uint16_t word = 3689;
        int64_t value = 17;
        unsigned width = 6;
        unsigned lsb = 3;
        unsigned lsb2 = 11;
        unsigned w2 = 10;

        print_binary(dummy1, word, false);
        print_binary(dummy1, value, false);
        // uint64_t a  = Bitpack_news(word, width, lsb, value);
        // print_binary(a, dummy2, true);
        // printf("%lu\n", a);

        // uint64_t b = Bitpack_newu(word, width, lsb, (uint64_t)value);
        // print_binary(b, dummy2, true);
        // printf("%lu\n", b);


        assert(Bitpack_gets(Bitpack_news(word, width, lsb, value), width, lsb) == value);
        

        assert(Bitpack_getu(Bitpack_newu(word, width, lsb, value), w2, lsb2) == Bitpack_getu(word, w2, lsb2));

        printf("successful\n");

        return 0;
}
