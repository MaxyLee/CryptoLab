#include "sm3.h"

u64 sm3_padding(u8* in, u64 bytenum) {
    u64 currbyte = bytenum;
    u64 bits = bytenum << 3;

    in[currbyte++] = 0x80;
    while(currbyte % 64 != 56) {
        in[currbyte++] = 0;
    }
    for(int i = 0; i < 8; i++) {
        in[currbyte++] = bits >> (56 - 8 * i);
    }

    return currbyte;
}

u32 ROL(u32 x, int n) {
    return (x << n) | (x >> (32 - n));
}

u32 T(int j) {
    return j < 16 ? 0x79cc4519 : 0x7a879d8a;
}

u32 FF(u32 x, u32 y, u32 z, int j) {
    return j < 16 ? x ^ y ^ z : (x & y) | (x & z) | (y & z);
}

u32 GG(u32 x, u32 y, u32 z, int j) {
    return j < 16 ? x ^ y ^ z : (x & y) | ((~x) & z);
}

u32 P0(u32 x) {
    return x ^ ROL(x, 9) ^ ROL(x, 17);
}

u32 P1(u32 x) {
    return x ^ ROL(x, 15) ^ ROL(x, 23);
}

void CF(u32* V, u32* block) {
    u32 W[68] = {};
    u32 W_prime[64] = {};
    u32 Reg[8] = {};

    for(int i = 0; i < 16; i++) {
        W[i] = block[i];
        // printf("W[%d]: 0x%08x\n", i, W[i]);
    } 
    for(int i = 16; i < 68; i++) {
        W[i] = P1(W[i - 16] ^ W[i - 9] ^ ROL(W[i - 3], 15)) ^ ROL(W[i - 13], 7) ^ W[i - 6];
        // printf("W[%d]: 0x%08x\n", i, W[i]);
    }
    for(int i = 0; i < 64; i++) {
        W_prime[i] = W[i] ^ W[i + 4];
    }

    for(int i = 0; i < 8; i++) {
        Reg[i] = V[i];
    }
    for(int i = 0; i < 64; i++) {
        u32 SS1 = ROL(ROL(Reg[0], 12) + Reg[4] + ROL(T(i), i % 32), 7);
        u32 SS2 = SS1 ^ ROL(Reg[0], 12);
        u32 TT1 = FF(Reg[0], Reg[1], Reg[2], i) + Reg[3] + SS2 + W_prime[i];
        u32 TT2 = GG(Reg[4], Reg[5], Reg[6], i) + Reg[7] + SS1 + W[i];
        Reg[3] = Reg[2];
        Reg[2] = ROL(Reg[1], 9);
        Reg[1] = Reg[0];
        Reg[0] = TT1;
        Reg[7] = Reg[6];
        Reg[6] = ROL(Reg[5], 19);
        Reg[5] = Reg[4];
        Reg[4] = P0(TT2);
    }
    for(int i = 0; i < 8; i++) {
        V[i] = V[i] ^ Reg[i];
    }
}

void SM3(u8* in, u32* out, u64 bytenum) {
    u64 currbyte = sm3_padding(in, bytenum);
    u64 blocknum = currbyte / 64;
    u32 V[8] = {0x7380166f, 0x4914b2b9, 0x172442d7, 0xda8a0600, 0xa96f30bc, 0x163138aa, 0xe38dee4d, 0xb0fb0e4e};

    // for(int i = 0; i < 64; i++) {
    //     printf("in[%d]: 0x%02x\n", i, in[i]);
    // }

    for(int i = 0; i < blocknum; i++) {
        u32 block[16] = {};
        for(int j = 0; j < 64; j ++) {
            block[j / 4] = (block[j / 4] << 8) + u32(in[i * 64 + j]);
        }
        CF(V, block);
    }

    for(int i = 0; i < 8; i++) {
        out[i] = V[i];
    }
}

#ifdef DEBUG
void sm3_debug() {
    u8 in[64] = {0x61, 0x62, 0x63};
    u32 out[8] = {};
    u64 bytenum = 3;

    SM3(in, out, bytenum);

    for(int i = 0; i < 8; i++) {
        printf("out[%d]: 0x%08x\n", i, out[i]);
    }
}
#endif