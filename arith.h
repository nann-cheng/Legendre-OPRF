#ifndef ARITHMETIC_H
#define ARITHMETIC_H

#include "parameters.h"
#include "random/random.h"

#define ALT_128 0


#define MASK(end, start) (((-(1ULL)) >> (64 - (end - start))) << start) // Compute mask from start bit to end-1 bit


// BASIC ARITHMETIC OPERATIONS ON SINGLE WORDS

// Digit multiplication
#define MUL(a, b, hi, lo){                                                                  \
    uint128_t t0 = (uint128_t)(a) * (uint128_t)(b);                                         \
    *(hi) = (uint64_t)(t0 >> RADIX);                                                        \
    (lo) = (uint64_t)t0;                                                                    \
}

// Digit addition with carry
#define ADDC(carry, a, b, c){                                                               \
    uint128_t temp = (uint128_t)(a) + (uint128_t)(b) + (uint128_t)(carry);                    \
    (carry) = (uint64_t)(temp >> RADIX);                                                      \
    (c) = (uint64_t)temp;                                                                     \
}

// Digit subtraction with borrow
#define SUBC(borrow, a, b, c){                                                              \
    uint128_t temp2 = (uint128_t)(a) - (uint128_t)(b) - (uint128_t)(borrow);                   \
    (borrow) = (uint64_t)(temp2 >> (sizeof(uint128_t) * 8 - 1));                               \
    (c) = (uint64_t)temp2;                                                                     \
}



// Print a field element
void print_f_elm(const f_elm_t a);

// Print a double size mp integer
void print_mp_elm(const digit_t* a, const int nwords);

// Print out an array of nbytes bytes as hex
void print_hex(const unsigned char* a, const int nbytes);



// Compare two field elements for equality, 0 if equal, 0xFF otherwise
uint8_t f_eq(const f_elm_t a, const f_elm_t b);

// Copy a field element
void f_copy(const f_elm_t a, f_elm_t b);

// Correction, i.e., reduction modulo p
void f_corr(f_elm_t a);

// Generate a random field element
void f_rand(f_elm_t a);

// // Convert a number from value to Montgomery form  (a -> aR)
// void to_mont(const digit_t* a, f_elm_t b);

// // Convert a number from Montgomery form into value (aR -> a)
// void from_mont(const f_elm_t a, digit_t* b);

// // Addition of two field elements
// void f_add(const f_elm_t a, const f_elm_t b, f_elm_t c);

// // Subtraction of two field elements
// void f_sub(const f_elm_t a, const f_elm_t b, f_elm_t c);

// // Negation of a field element
// void f_neg(const f_elm_t a, f_elm_t b);

// // Multiplication of two multiprecision words (without reduction)
// void mp_mul(const uint64_t* a, const uint64_t* b, uint64_t* c);

// // Montgomery form reduction after multiplication
// void rdc_mont(const uint64_t* a, uint64_t* c);

// // Multiplication of field elements
// void f_mul(const f_elm_t a, const f_elm_t b, f_elm_t c);

#endif