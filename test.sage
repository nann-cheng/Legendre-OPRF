import random

NBITS_PRIME = 128
NBITS_WORD = 64
NWORDS_PRIME = NBITS_PRIME//NBITS_WORD

w = 2^NBITS_WORD
R = 2^NBITS_PRIME

# p = R.previous_prime()
p = (R- 2^(NBITS_PRIME - 1)).next_prime()
# p = (2^(NBITS_PRIME - 1)).previous_prime()

RR = pow(R, 1, p)
mR = pow(-R, 1, p)
R2 = pow(R, 2, p)
iR = pow(R, -1, p)
pp = pow(-p,-1, R)
ip = pow(p, -1, R)

F = GF(p)

def legg(a):
    b = F(a)^((p-1)/2)
    if(b == -1):
        return 1
    else:
        return 0

def hexx(a):
    return ('0x%032X' % a)

def trail_zeros(n):
    i = 0
    while ((n % 2) == 0):
        n = n / 2
        i+= 1
    return i



def string_field_elm(b):
    a = b
    arr = []
    for _ in range(NWORDS_PRIME):
        arr.append("0x%016X" % (b%w))
        b >>= NBITS_WORD

    return("{" + ", ".join(arr) + "}")







def main():

    sp = string_field_elm(p)
    sRR = string_field_elm(RR)
    sR2 = string_field_elm(R2)
    siR = string_field_elm(iR)
    spp = string_field_elm(pp)
    sip = string_field_elm(ip)
    sz = string_field_elm(0)
    so = string_field_elm(1)
    print("\tstatic const digit_t p[WORDS_FIELD]         = " + sp + ";\t// Field order p")
    print("\tstatic const digit_t Mont_one[WORDS_FIELD]  = " + sRR + ";\t// R  =  2^{NBITS_PRIME} (mod p)")
    print("\tstatic const digit_t R2[WORDS_FIELD]        = " + sR2 + ";\t// R2 = (2^{NBITS_PRIME})^2 (mod p)")
    print("\tstatic const digit_t iR[WORDS_FIELD]        = " + siR + ";\t// iR =  R^(-1) (mod p)")
    print("\tstatic const digit_t pp[WORDS_FIELD]        = " + spp + ";\t// pp = -p^(-1) mod R")
    print("\tstatic const digit_t ip[WORDS_FIELD]        = " + sip + ";\t// ip =  p^(-1) mod R")
    print("\tstatic const digit_t Zero[WORDS_FIELD]      = " + sz  + ";\t// 0 ")
    print("\tstatic const digit_t One[WORDS_FIELD]       = " + so  + ";\t// 1 ")

    print()
    print()

    print("p\t0b " + bin(p)[2:].zfill(NBITS_PRIME))
    print("p-2\t0b " + bin(p-2)[2:].zfill(NBITS_PRIME))
    print("(p-1)/2\t0b " + bin((p-1)/2)[2:].zfill(NBITS_PRIME))

    print()

    print("+R \t0b " + bin(RR)[2:].zfill(NBITS_PRIME))
    print("-R \t0b " + bin(mR)[2:].zfill(NBITS_PRIME))


if __name__ == "__main__":
    main()


# def leg(a, b):
#     out = 0
#     if (a == 0):
#         return out

#     c = trail_zeros(a);
#     out = c & (b ^^ (b >> 1));
#     b >>= 1

#     # /* We may have c==GMP_LIMB_BITS-1, so we can't use a>>c+1. */
#     a = a >> (c+1);
#     j = 0
#     while(True):
#         t = (a - b) % R
#         b_gt_a = R-1 if (t & 2^127) else 0x00

#         # /* If b > a, invoke reciprocity */
#         out = out ^^ (b_gt_a & a & b)

#         # /* b <-- min (a, b) */
#         b = (b + (b_gt_a & t)) % R

#         # /* a <-- |a - b| */
#         a = ((t ^^ b_gt_a) - b_gt_a) % R

#         # /* Number of trailing zeros is the same no matter if we look at
#         # * t or a, but using t gives more parallelism. */
#         c = trail_zeros(t)
#         c = c + 1

#         # /* (2/b) = -1 if b = 3 or 5 mod 8 */
#         out = out ^^ (c & (b ^^ (b >> 1)))
#         a = a >> c
#         j += 1

#         if(not (a > 0)):
#             break

#     return out & 1


