#include "des.h"
#include <cstring>

u64 subkeys[16] = {};

void gen_subkeys(u64 key, bool encrypt) {
    //generate subkeys
    u32 C = 0;
    u32 D = 0;

    //PC-1 sub
    for (int i = 0; i < 28; i++) {
        C = (C << 1) + ((key >> (64 - PC1[i])) & 1);
    }
    for (int i = 0; i < 28; i++) {
        D = (D << 1) + ((key >> (64 - PC1[28 + i])) & 1);
    }

    for (int i = 0; i < 16; i++) {
        if(i == 0 || i == 1 || i == 8 || i == 15) {
            C = ((C << 1) & 0xffffffe) + ((C >> 27) & 1);
            D = ((D << 1) & 0xffffffe) + ((D >> 27) & 1);
        } else {
            C = ((C << 2) & 0xffffffc) + ((C >> 26) & 0b11);
            D = ((D << 2) & 0xffffffc) + ((D >> 26) & 0b11);
        }
        u64 CD = (u64(C) << 28) + D;
        for (int j = 0; j < 48; j++) {
            if (encrypt) {
                subkeys[i] = (subkeys[i] << 1) + ((CD >> (56 - PC2[j])) & 1);
            } else {
                subkeys[15 - i] = (subkeys[15 - i] << 1) + ((CD >> (56 - PC2[j])) & 1);
            }
        }
        
    }
}

u32 f(u32 halfblock, u64 subkey) {
    u64 expansion = 0;
    u8 split[8] = {};
    u32 substitution = 0;
    u32 permutation = 0;

    for (int i = 0; i < 48; i++) {
        expansion = (expansion << 1) + ((halfblock >> (32 - E[i])) & 1);
    }
    expansion = expansion ^ subkey;
    for (int i = 0; i < 8; i++) {
        split[i] = ((expansion >> (42 - i * 6)) & 0b111111);
        u8 row, col;
        row = (((split[i] >> 5) & 1) << 1) + (split[i] & 1);
        col = (split[i] >> 1) & 0b1111;
        substitution += u32(SBox[i][row * 16 + col] & 0b1111) << ((7 - i) * 4);
    }
    for (int i = 0; i < 32; i++) {
        permutation += ((substitution >> (32 - PBox[i])) & 1) << (31 - i);
    }
    return permutation;
}

u64 DES(u64 in) {
    u32 L[17] = {};
    u32 R[17] = {};
    u64 out = 0;

    //IP
    for (int i = 0; i < 32; i++) {
        L[0] = (L[0] << 1) + ((in >> (64 - IP[i])) & 1);
    }
    for (int i = 0; i < 32; i++) {
        R[0] = (R[0] << 1) + ((in >> (64 - IP[32 + i])) & 1);
    }

    //16 iterations
    for (int i = 1; i < 17; i++) {
        L[i] = R[i - 1];
        R[i] = L[i - 1] ^ f(R[i - 1], subkeys[i - 1]);
    }

    //IP-1
    u64 RL = (u64(R[16]) << 32) + L[16];
    for (int i = 0; i < 64; i++) {
        out = (out << 1) + ((RL >> (64 - IP_1[i])) & 1);
    }
    return out;
}

void DES_CBC(u64 iv, u64 key, u64* in, u64* out, u64 length,bool encrypt) {
    u64 tmp = iv;
    memset(subkeys, 0, sizeof(subkeys));
    gen_subkeys(key, encrypt);
    if(encrypt) {
        for(int i = 0; i < length; i++) {
            out[i] = DES(in[i] ^ tmp);
            tmp = out[i];
        }
    } else {
        for(int i = 0; i < length; i++) {
            out[i] = DES(in[i]) ^ tmp;
            tmp = in[i];
        }
    }
}

#ifdef DEBUG
void des_debug() {
    // u64 plain = 0b100100001100101011011000110110001101111001000000100100000000000;
    u64 plain = 0b0000000100100011010001010110011110001001101010111100110111101111;
    // u64 plain = 0x0123456789abcdef;
    // u64 key = 0b101000001100001011100110111001101010111011011110111010000000000;
    u64 key = 0b0001001100110100010101110111100110011011101111001101111111110001;

    memset(subkeys, 0, sizeof(subkeys));
    gen_subkeys(key, true);

    u64 out = DES(plain);

    // memset(subkeys, 0, sizeof(subkeys));
    // gen_subkeys(key, false);

    // u64 out2 = DES(out);

    printf("debug:\n");
    printf("plaintext: 0x%016llx\n", plain);
    printf("key: 0x%016llx\n", key);
    // for(int i = 0; i < 16; i++) {
    //     printf("subkey[%d]: 0x%llx\n", i, subkeys[i]);
    // }
    printf("cipher: 0x%llx\n", out);
    // printf("out2: 0x%llx\n", out2);
    }

#endif