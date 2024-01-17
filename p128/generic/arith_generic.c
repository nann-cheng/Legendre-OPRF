#include "../arith128.h"


// Correction, i.e., reduction modulo p
// a in [0, 2 * p - 1] -> a in [0, p-1]
void f_red(f_elm_t a)
{
    digit_t mask, borrow = 0, carry = 0;
    for (int i = 0; i < WORDS_FIELD; i++)
    {
        SUBC(borrow, a[i], p[i], a[i]);
    }

    mask = 0 - borrow;

    for (int i = 0; i < WORDS_FIELD; i++)
        ADDC(carry, a[i], p[i] & mask, a[i]);

    // Alternative reduction
    // digit_t mask, borrow = 0, carry = 0;
    // for (int i = 0; i < WORDS_FIELD; i++)
    // {
    //     ADDC(borrow, a[i], Mont_one[i], a[i]);
    // }

    // mask = 0 - (1-borrow);

    // for (int i = 0; i < WORDS_FIELD; i++)
    //     SUBC(carry, a[i], Mont_one[i] & mask, a[i]);
}

// Generate a random field element
void f_rand(f_elm_t a)
{
    randombytes((unsigned char *)a, sizeof(digit_t) * WORDS_FIELD);
    f_red(a); // Not uniformly random, can use rejection sampling to fix, thought it's pretty close to uniform
}

// Addition of two field elements
void f_add(const f_elm_t a, const f_elm_t b, f_elm_t c)
{
    digit_t mask, carry = 0;

    for (int i = 0; i < WORDS_FIELD; i++)
        ADDC(carry, a[i], b[i], c[i]);

    mask = 0 - carry;
    carry = 0;
    for (int i = 0; i < WORDS_FIELD; i++)
        ADDC(carry, c[i], Mont_one[i] & mask, c[i]);

    f_red(c);
}

// Subtraction of two field elements
void f_sub(const f_elm_t a, const f_elm_t b, f_elm_t c)
{
    digit_t mask, borrow = 0, carry = 0;

    for (int i = 0; i < WORDS_FIELD; i++)
        SUBC(borrow, a[i], b[i], c[i]);

    mask = 0 - borrow;

    for (int i = 0; i < WORDS_FIELD; i++)
        SUBC(carry, c[i], Mont_one[i] & mask, c[i])

    f_red(c);
}

// Negation of a field element
void f_neg(const f_elm_t a, f_elm_t b)
{
    digit_t borrow = 0;

    for (int i = 0; i < WORDS_FIELD; i++)
        SUBC(borrow, p[i], a[i], b[i]);

    f_red(b);
}

// Multiplication of two multiprecision words (without reduction)
void mp_mul(const digit_t *a, const digit_t *b, digit_t *c)
{ // Schoolbook multiplication
    digit_t carry, UV[2], t = 0, u = 0, v = 0;

    for (int i = 0; i < WORDS_FIELD; i++)
    {
        for (int j = 0; j <= i; j++)
        {
            carry = 0;
            MUL(a[j], b[i - j], UV + 1, UV[0]);
            ADDC(carry, UV[0], v, v);
            ADDC(carry, UV[1], u, u);
            t += carry;
        }
        c[i] = v;
        v = u;
        u = t;
        t = 0;
    }

    for (int i = WORDS_FIELD; i < 2 * WORDS_FIELD - 1; i++)
    {
        for (int j = i - WORDS_FIELD + 1; j < WORDS_FIELD; j++)
        {
            carry = 0;
            MUL(a[j], b[i - j], UV + 1, UV[0]);
            ADDC(carry, UV[0], v, v);
            ADDC(carry, UV[1], u, u);
            t += carry;
        }
        c[i] = v;
        v = u;
        u = t;
        t = 0;
    }
    c[2 * WORDS_FIELD - 1] = v;
}


// Montgomery form reduction after multiplication
void mont_redc(const digit_t *a, digit_t *c)
{
    // c = a*R^-1 mod p, where R = 2^256.
    // If a < 2^256*p, the output c is in the range [0, p).
    // a is assumed to be in Montgomery representation.
    digit_t mask, carry = 0;
    digit_t t0[2 * WORDS_FIELD], t1[WORDS_FIELD];

    for (int i = 0; i < WORDS_FIELD; i++)
        c[i] = a[i];

    mp_mul(c, ip, t0);
    f_copy(t0, t1);
    mp_mul(t1, p, t0);

    for (int i = WORDS_FIELD; i < 2 * WORDS_FIELD; i++)
        SUBC(carry, a[i], t0[i], t0[i]);


    mask = 0 - carry;
    carry = 0;
    for (int i = 0; i < WORDS_FIELD; i++)
        ADDC(carry, t0[WORDS_FIELD + i], p[i] & mask, c[i]);

}



// Multiplication of field elements
void f_mul(const f_elm_t a, const f_elm_t b, f_elm_t c)
{
    digit_t t0[2 * WORDS_FIELD] = {0};

    mp_mul(a, b, t0);
    mont_redc(t0, c);
}



// Convert a number from value to Montgomery form  (a -> aR)
void to_mont(const digit_t *a, f_elm_t b)
{
    f_mul(a, R2, b);
}


// Convert a number from Montgomery form into value (aR -> a)
void from_mont(const f_elm_t a, digit_t *b)
{
    digit_t t0[2 * WORDS_FIELD] = {0};
    f_copy(a, t0);
    mont_redc(t0, b);
}



#if ALT_128

// Multiplicative inverse of a field element
void f_inv(const f_elm_t a, f_elm_t b)
{

    f_elm_t t0;
    unsigned int i;
    f_copy(Mont_one, t0);

    /* p - 2 =  1000000000000000\
                0000000000000000\
                0000000000000000\
                0000000000000000\
                0000000000000000\
                0000000000000000\
                0000000000000000\
                0000000000011011\
    */

    // bit = 127
    f_mul(t0, a, t0);

    // bits 126 down to 5
    for (i = 126; i > 4; i--)
    {
        f_mul(t0, t0, t0);
        // f_mul(t0, a, t0);
    }

    // bit = 4
    f_mul(t0, t0, t0);
    f_mul(t0, a, t0);

    // bit = 3
    f_mul(t0, t0, t0);
    f_mul(t0, a, t0);

    // bit = 2
    f_mul(t0, t0, t0);

    // bit = 1
    f_mul(t0, t0, t0);
    f_mul(t0, a, t0);

    // bit = 0
    f_mul(t0, t0, t0);
    f_mul(t0, a, t0);

    f_copy(t0, b);

}


// Legendre symbol of a field element
void f_leg(const f_elm_t a, unsigned char *b)
{

    unsigned int i;
    f_elm_t t0;
    f_copy(Mont_one, t0);
    // Compute a^((p-1)/2)

    /* (p - 1)/2 = 0b   01000000 00000000\
                        00000000 00000000\
                        00000000 00000000\
                        00000000 00000000\
                        00000000 00000000\
                        00000000 00000000\
                        00000000 00000000\
                        00000000 00001110\
    */


    // bit = 127 (=0)

    // bit = 126
    f_mul(t0, a, t0);

    // bits 125 down to 4
    for (i = 125; i > 3; i--)
    {
        f_mul(t0, t0, t0);
    }

    // bit = 3
    f_mul(t0, t0, t0);
    f_mul(t0, a, t0);

    // bit = 2
    f_mul(t0, t0, t0);
    f_mul(t0, a, t0);

    // bit = 1
    f_mul(t0, t0, t0);
    f_mul(t0, a, t0);

    // bit = 0
    f_mul(t0, t0, t0);

    *b = ((*(unsigned char *)t0) & 0x08) >> 3;

}

#else


// Multiplicative inverse of a field element
void f_inv(const f_elm_t a, f_elm_t b)
{

    f_elm_t t[6];
    unsigned int i, j;

    f_copy(a, t[0]);
    f_copy(a, t[1]);

    /* p - 2 = 0b   11111111 11111111\
                    11111111 11111111\
                    11111111 11111111\
                    11111111 11111111\

                    11111111 11111111\
                    11111111 11111111\

                    11111111 11111111\
                    11111111 0101 1111\
    */

    // First 64 bits = 2^6 bits
    for(j = 0; j < 6; j++){
        for (i = 0; i < (1 << j); i++)
            f_mul(t[0], t[0], t[0]);
        f_mul(t[0], t[1], t[0]);
        if(j == 1) f_copy(t[0], t[2]);  // a^(2^4  - 1) = a^0b 1111
        if(j == 2) f_copy(t[0], t[3]);  // a^(2^8  - 1) = a^0b 11111111
        if(j == 3) f_copy(t[0], t[4]);  // a^(2^16 - 1) = a^0b 11111111 11111111
        if(j == 4) f_copy(t[0], t[5]);  // a^(2^32 - 1) = a^0b 11111111 11111111 11111111 11111111
        f_copy(t[0], t[1]);             // a^(2^(2^(j+1)) - 1)
    }
    
    /* t[3] = a ^ 0b    11111111 11111111
                        11111111 11111111
    */

    // Next 32 bits
    for (i = 0; i < 32; i++)
        f_mul(t[0], t[0], t[0]);
    f_mul(t[0], t[5], t[0]);

    // Next 16 bits
    for (i = 0; i < 16; i++)
        f_mul(t[0], t[0], t[0]);
    f_mul(t[0], t[4], t[0]);

    // Next 8 bits
    for (i = 0; i < 8; i++)
        f_mul(t[0], t[0], t[0]);
    f_mul(t[0], t[3], t[0]);

    // bit = 7
    f_mul(t[0], t[0], t[0]);
    // bit = 6
    f_mul(t[0], t[0], t[0]);
    f_mul(t[0], a, t[0]);
    // bit = 5
    f_mul(t[0], t[0], t[0]);
    // bit = 4
    f_mul(t[0], t[0], t[0]);
    f_mul(t[0], a, t[0]);
    // Last 4 bits
    for (i = 0; i < 4; i++)
        f_mul(t[0], t[0], t[0]);
    f_mul(t[0], t[2], t[0]);

    f_copy(t[0], b);

}


// Legendre symbol of a field element
void f_leg(const f_elm_t a, unsigned char *b)
{

    f_elm_t t[5];
    unsigned int i, j;

    f_copy(a, t[0]);
    f_copy(a, t[1]);

    /* (p - 1)/2 = 0b   01111111 11111111\
                        11111111 11111111\
                        11111111 11111111\
                        11111111 11111111\

                        11111111 11111111\
                        11111111 11111111\

                        11111111 11111111\
                        11111111 1 0110000\
    */


    // bit = 128 (=0)

    // First 64 bits = 2^6 bits
    for(j = 0; j < 6; j++){
        for (i = 0; i < (1 << j); i++)
            f_mul(t[0], t[0], t[0]);
        f_mul(t[0], t[1], t[0]);
        if(j == 2) f_copy(t[0], t[2]);  // a^(2^8  - 1) = a^0b 11111111
        if(j == 3) f_copy(t[0], t[3]);  // a^(2^16 - 1) = a^0b 11111111 11111111
        if(j == 4) f_copy(t[0], t[4]);  // a^(2^32 - 1) = a^0b 11111111 11111111 11111111 11111111
        f_copy(t[0], t[1]);             // a^(2^(2^(j+1)) - 1)
    }
    
    /* t[3] = a ^ 0b    11111111 11111111
                        11111111 11111111
    */

    // Next 32 bits
    for (i = 0; i < 32; i++)
        f_mul(t[0], t[0], t[0]);
    f_mul(t[0], t[4], t[0]);

    // Next 16 bits
    for (i = 0; i < 16; i++)
        f_mul(t[0], t[0], t[0]);
    f_mul(t[0], t[3], t[0]);

    // Next 8 bits
    for (i = 0; i < 8; i++)
        f_mul(t[0], t[0], t[0]);
    f_mul(t[0], t[2], t[0]);

    // bit = 6
    f_mul(t[0], t[0], t[0]);
    // bit = 5
    f_mul(t[0], t[0], t[0]);
    f_mul(t[0], a, t[0]);
    // bit = 4
    f_mul(t[0], t[0], t[0]);
    f_mul(t[0], a, t[0]);
    // bit = 3
    f_mul(t[0], t[0], t[0]);
    // bit = 2
    f_mul(t[0], t[0], t[0]);
    // bit = 1
    f_mul(t[0], t[0], t[0]);
    // bit = 0
    f_mul(t[0], t[0], t[0]);

    *b = ((*(unsigned char *)t[0]) & 0x40) >> 6;

}

#endif
