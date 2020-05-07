#include "sha2.h"
#include <cstring>

u64 padding(u32* in, u64 bytenum) {///////////////////////////
    // u64 currbyte = bytenum;
    u64 currlen = bytenum / 4 + 1;
    u64 index = bytenum % 4;
    if(index == 0) {
        in[currlen] = 0x80000000;
    } else {
        in[currlen - 1] += u32(0x80000000) >> (index * 8); 
    }
    // if(currbyte % 16 != 14) {
    //     in[currbyte++] = 0x80000000;
    // }
    while(currlen % 16 != 14) {
        in[currlen++] = 0;
    }
    u64 bits = bytenum << 3;
    // printf("currbyte: %u\n", currbyte);
    in[currlen++] = bits >> 32;
    in[currlen++] = bits;
    return currlen;
}

u32 S(u32 x, int n) {
    n = n % 32;
    return (x >> n) | (x << (32 - n));
}

u32 Ch(u32 x, u32 y, u32 z) {
    return (x & y) ^ ((~x) & z);
}

u32 Ma(u32 x, u32 y, u32 z) {
    return (x & y) ^ (x & z) ^ (y & z);
}

u32 Sigma_0(u32 x) {
    return S(x, 2) ^ S(x, 13) ^ S(x, 22);
}

u32 Sigma_1(u32 x) {
    return S(x, 6) ^ S(x, 11) ^ S(x, 25);
}

u32 sigma_0(u32 x) {
    return S(x, 7) ^ S(x, 18) ^ (x >> 3);
}

u32 sigma_1(u32 x) {
    return S(x, 17) ^ S(x, 19) ^ (x >> 10);
}

void SHA256(u32* in, u32* out, u64 bytenum) {
    u64 currlen = padding(in, bytenum);
    u32 H[8] = {0x6a09e667, 0xbb67ae85, 0x3c6ef372, 0xa54ff53a, 0x510e527f, 0x9b05688c, 0x1f83d9ab, 0x5be0cd19};
    u32 W[64] = {};

    // for(int i = 0; i < 16; i++) {
    //     printf("in[%d]: 0x%x\n", i, in[i]);
    // }
    // printf("currbyte: %u\n", currlen);

    u64 blocknum = currlen / 16;
    for(int i = 0; i < blocknum; i++) {
        for(int j = 0; j < 16; j++) {
            W[j] = in[i * 16 + j];
        }
        for(int j = 16; j < 64; j++) {
            W[j] = sigma_1(W[j - 2]) + W[j - 7] + sigma_0(W[j - 15]) + W[j - 16];
        }
        u32 newH[8];
        for(int j = 0; j < 8; j++) {
            newH[j] = H[j];
        }
        for(int j = 0; j < 64; j++) {
            u32 s0 = Sigma_0(newH[0]);
            u32 maj = Ma(newH[0], newH[1], newH[2]);
            u32 t2 = s0 + maj;
            u32 s1 = Sigma_1(newH[4]);
            u32 ch = Ch(newH[4], newH[5], newH[6]);
            u32 t1 = newH[7] + s1 + ch + K[j] + W[j];
            for(int k = 6; k >= 0; k--) {
                newH[k + 1] = newH[k];
            }
            newH[4] += t1;
            newH[0] = t1 + t2;
        }
        for(int j = 0; j < 8; j++) {
            H[j] += newH[j];
        }
    }
    for(int i = 0; i < 8; i++) {
        out[i] = H[i];
    }
}

#ifdef DEBUG
void sha2_debug() {
    u32 in[16] = {0x61626300};
    u32 out[8] = {};
    u64 bytenum = 3;
    SHA256(in, out, bytenum);
    printf("SHA256: 0x");
    for(int i = 0; i < 8; i++) {
        printf("%x", out[i]);
    }
    printf("\n");
}
#endif