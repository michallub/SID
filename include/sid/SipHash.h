#ifndef SIDSIPHASH_H
#define SIDSIPHASH_H

//ORIGINAL IMPLEMENTATION MODIFIED TO USE AS CONSTEXPR

/*
   SipHash reference C implementation

   Copyright (c) 2012-2021 Jean-Philippe Aumasson
   <jeanphilippe.aumasson@gmail.com>
   Copyright (c) 2012-2014 Daniel J. Bernstein <djb@cr.yp.to>

   To the extent possible under law, the author(s) have dedicated all copyright
   and related and neighboring rights to this software to the public domain
   worldwide. This software is distributed without any warranty.

   You should have received a copy of the CC0 Public Domain Dedication along
   with
   this software. If not, see
   <http://creativecommons.org/publicdomain/zero/1.0/>.
 */

#include <inttypes.h>
#include <string.h>

#include <cassert>
#include <stdint.h>
#include <stdio.h>

 /* default: SipHash-2-4 */
#ifndef cROUNDS
#define cROUNDS 2
#endif

#ifndef dROUNDS
#define dROUNDS 4
#endif

#define ROTL(x, b) (uint64_t)(((x) << (b)) | ((x) >> (64 - (b))))

#define U32TO8_LE(p, v)                                                        \
    (p)[0] = (uint8_t)((v));                                                   \
    (p)[1] = (uint8_t)((v) >> 8);                                              \
    (p)[2] = (uint8_t)((v) >> 16);                                             \
    (p)[3] = (uint8_t)((v) >> 24);

#define U64TO8_LE(p, v)                                                        \
    U32TO8_LE((p), (uint32_t)((v)));                                           \
    U32TO8_LE((p) + 4, (uint32_t)((v) >> 32));

#define U8TO64_LE(p)                                                           \
    (((uint64_t)(uint8_t((p)[0])))       | ((uint64_t)(uint8_t((p)[1])) << 8 ) |                        \
     ((uint64_t)(uint8_t((p)[2])) << 16) | ((uint64_t)(uint8_t((p)[3])) << 24) |                 \
     ((uint64_t)(uint8_t((p)[4])) << 32) | ((uint64_t)(uint8_t((p)[5])) << 40) |                 \
     ((uint64_t)(uint8_t((p)[6])) << 48) | ((uint64_t)(uint8_t((p)[7])) << 56))

#define SIPROUND                                                               \
    do {                                                                       \
        v0 += v1;                                                              \
        v1 = ROTL(v1, 13);                                                     \
        v1 ^= v0;                                                              \
        v0 = ROTL(v0, 32);                                                     \
        v2 += v3;                                                              \
        v3 = ROTL(v3, 16);                                                     \
        v3 ^= v2;                                                              \
        v0 += v3;                                                              \
        v3 = ROTL(v3, 21);                                                     \
        v3 ^= v0;                                                              \
        v2 += v1;                                                              \
        v1 = ROTL(v1, 17);                                                     \
        v1 ^= v2;                                                              \
        v2 = ROTL(v2, 32);                                                     \
    } while (0)

#ifdef DEBUG
#define TRACE                                                                  \
    do {                                                                       \
        printf("(%3zu) v0 %016" PRIx64 "\n", inlen, v0);                       \
        printf("(%3zu) v1 %016" PRIx64 "\n", inlen, v1);                       \
        printf("(%3zu) v2 %016" PRIx64 "\n", inlen, v2);                       \
        printf("(%3zu) v3 %016" PRIx64 "\n", inlen, v3);                       \
    } while (0)
#else
#define TRACE
#endif

template <typename inputbytetype>
constexpr int siphash(const inputbytetype * in, const size_t inlen, const uint8_t * k, uint8_t* out,
    const size_t outlen) {

    const inputbytetype* ni = in;
    const unsigned char* kk = (const unsigned char*)k;
    static_assert(sizeof(inputbytetype) == 1, "error: siphash inputbytetype is not byte");
    //assert((outlen == 8) || (outlen == 16));
    uint64_t v0 = UINT64_C(0x736f6d6570736575);
    uint64_t v1 = UINT64_C(0x646f72616e646f6d);
    uint64_t v2 = UINT64_C(0x6c7967656e657261);
    uint64_t v3 = UINT64_C(0x7465646279746573);
    uint64_t k0 = U8TO64_LE(kk);
    uint64_t k1 = U8TO64_LE(kk + 8);
    uint64_t m = {};
    int i = {};
    const inputbytetype* end = ni + inlen - (inlen % sizeof(uint64_t));
    const int left = inlen & 7;
    uint64_t b = ((uint64_t)inlen) << 56;
    v3 ^= k1;
    v2 ^= k0;
    v1 ^= k1;
    v0 ^= k0;

    if (outlen == 16)
        v1 ^= 0xee;

    for (; ni != end; ni += 8) {
        m = U8TO64_LE(ni);
        v3 ^= m;

        for (i = 0; i < cROUNDS; ++i)
            SIPROUND;

        v0 ^= m;
    }

    switch (left) {
    case 7:
        b |= ((uint64_t)(uint8_t)ni[6]) << 48;
    case 6:
        b |= ((uint64_t)(uint8_t)ni[5]) << 40;
    case 5:
        b |= ((uint64_t)(uint8_t)ni[4]) << 32;
    case 4:
        b |= ((uint64_t)(uint8_t)ni[3]) << 24;
    case 3:
        b |= ((uint64_t)(uint8_t)ni[2]) << 16;
    case 2:
        b |= ((uint64_t)(uint8_t)ni[1]) << 8;
    case 1:
        b |= ((uint64_t)(uint8_t)ni[0]);
        break;
    case 0:
        break;
    }

    v3 ^= b;

    for (i = 0; i < cROUNDS; ++i)
        SIPROUND;

    v0 ^= b;

    if (outlen == 16)
        v2 ^= 0xee;
    else
        v2 ^= 0xff;

    for (i = 0; i < dROUNDS; ++i)
        SIPROUND;

    b = v0 ^ v1 ^ v2 ^ v3;
    U64TO8_LE(out, b);

    if (outlen == 8)
        return 0;

    v1 ^= 0xdd;

    for (i = 0; i < dROUNDS; ++i)
        SIPROUND;

    b = v0 ^ v1 ^ v2 ^ v3;
    U64TO8_LE(out + 8, b);

    return 0;
}

struct Hash64x1 {
    uint64_t val[1];
};
struct Hash64x2 {
    uint64_t val[2];
};
struct Hash8x16 {
    uint8_t val[16];
};

template <typename inputbytetype>
constexpr Hash64x1 SipHash64(const inputbytetype* data, size_t datasize, Hash8x16 key) noexcept
{
    //uint8_t keyu8[16] = {
    //    key.val[0] >> 0 ,
    //    key.val[0] >> 8 ,
    //    key.val[0] >> 16,
    //    key.val[0] >> 24,
    //    key.val[0] >> 32,
    //    key.val[0] >> 40,
    //    key.val[0] >> 48,
    //    key.val[0] >> 56,
    //    key.val[1] >> 0 ,
    //    key.val[1] >> 8 ,
    //    key.val[1] >> 16,
    //    key.val[1] >> 24,
    //    key.val[1] >> 32,
    //    key.val[1] >> 40,
    //    key.val[1] >> 48,
    //    key.val[1] >> 56,
    // };

    uint8_t out[8] = {};

    siphash(data, datasize, &key.val[0], &out[0], 8);

    uint64_t result = 0;
    for (int i = 0; i < 8; i++)
        result = (result << 8) | out[i];
    return Hash64x1{ result };
}

template <typename inputbytetype>
constexpr Hash64x2 SipHash128(const inputbytetype* data, size_t datasize, Hash8x16 key) noexcept
{
    uint8_t out[16] = {};

    siphash(data, datasize, &key.val[0], &out[0], 8);

    uint64_t result1 = 0;
    uint64_t result2 = 0;

    for (int i = 0; i < 8; i++)
        result1 = (result1 << 8) | out[i];

    for (int i = 8; i < 16; i++)
        result2 = (result2 << 8) | out[i];

    return Hash64x2{ result1, result2 };
}

#endif
