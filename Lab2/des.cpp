#include "des.h"

u32 f(u32 halfblock, u64 subkey) {
    u64 expansion = 0;
    u8 split[8] = {};
    u32 substitution = 0;
    u32 permutation = 0;
    for (int i = 0; i < 48; i++) {
        expansion += ((halfblock >> E[i]) & 1) << (47 - i);
    }
    expansion = expansion ^ subkey;
    for (int i = 0; i < 8; i++) {
        split[i] = (expansion >> (i * 6) & 0b111111);
        u32 row, col;
        row = (((split[i] >> 5) & 1) << 1) + (split[i] & 1);
        col = (split[i] >> 1) & 0b1111;
        substitution += u32(SBox[i][row * 16 + col] & 0b1111) << ((7 - i) * 4);
    }
    for (int i = 0; i < 32; i++) {
        permutation += ((substitution >> (32 - PBox[i])) & 1) << (31 - i);
    }
    return permutation;
}

u64 DES(u64 key, u64 text, bool encrypt) {
    u32 C = 0;
    u32 D = 0;
    u64 subkeys[16] = {};
    u32 L[17] = {};
    u32 R[17] = {};
    u64 out = 0;

    //generate subkeys
    for (int i = 0; i < 28; i++) {
        C += ((key >> (64 - PC1[i])) & 1) << (27 - i);
    }
    for (int i = 0; i < 28; i++) {
        D += ((key >> (64 - PC1[28 + i])) & 1) << (27 - i);
    }
    for (int i = 0; i < 16; i++) {
        u64 CD = (u64(C) << 28) + D;
        for (int j = 0; j < 48; j++) {
            if (encrypt) {/////////////////////////////////////////////
                subkeys[i] += ((CD >> (56 - PC2[j])) & 1) << (47 - j);
            } else {
                subkeys[15 - i] += ((CD >> (56 - PC2[j])) & 1) << (47 - j);
            }
        }
        if(i == 0 || i == 1 || i == 8 || i == 15) {
            C <<= 1;
            C += (C >> 28) & 1;
            C = C & 0xfffffff;
            D <<= 1;
            D += (C >> 28) & 1;
            D = D & 0xfffffff;
        } else {
            C <<= 2;
            C += (C >> 28) & 0b11;
            C = C & 0xfffffff;
            D <<= 2;
            D += (C >> 28) & 0b11;
            D = D & 0xfffffff;
        }
    }

    //IP
    for (int i = 0; i < 32; i++) {
        L[0] += ((text >> (64 - IP[i])) & 1) << (31 - i);
    }
    for (int i = 0; i < 32; i++) {
        R[0] += ((text >> (64 - IP[32 + i])) & 1) << (31 - i);
    }

    //16 iterations
    for (int i = 1; i < 17; i++) {
        L[i] = R[i - 1];
        R[i] = L[i - 1] & f(R[i - 1], subkeys[i - 1]);
    }

    //IP-1
    u64 tmp = (u64(R[16]) << 32) + L[16];
    for (int i = 0; i < 64; i++) {
        out += ((tmp >> (64 - IP_1[i])) & 1) << (63 - i);
    }
    return out;
}