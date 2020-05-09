#include "sha3.h"
#include <cstring>

u64 padding(u8* in, u64 bytenum) {
    u64 currbyte = bytenum;
    // if(currbyte % 136 != 0) {
    in[currbyte++] = 0x06;
    // }
    while(currbyte % 136 != 0) {
        in[currbyte++] = 0;
    }
    in[currbyte - 1] += 0x80;
    return currbyte;
}

u64 reverse(u64 x) {
    u64 res = 0;
    for(int i = 0; i < 8; i++) {
        res = (res << 8) + ((x >> (i * 8)) & 0xff);
    }
    return res;
}

u64 ROL(u64 x, int n) {
    return (x << n) | (x >> (64 - n));
}

void Keccak_p(u64 A[5][5]) {
    for(int k = 0; k < 24; k++) {
        //Theta
        u64 C[5] = {};
        u64 D[5] = {};
        for(int i = 0; i < 5; i++) {
            C[i] = A[i][0] ^ A[i][1] ^ A[i][2] ^ A[i][3] ^ A[i][4];
        }
        for(int i = 0; i < 5; i++) {
            D[i] = C[(i + 4) % 5] ^ ROL(C[(i + 1) % 5], 1);
        }
        for(int i = 0; i < 5; i++) {
            for(int j = 0; j < 5; j++) {
                A[i][j] = A[i][j] ^ D[i];
            }
        }
        
        //rho & pi
        u64 B[5][5] = {};
        for(int i = 0; i < 5; i++) {
            for(int j = 0; j < 5; j++) {
                B[j][(2 * i + 3 * j) % 5] = ROL(A[i][j], r[i][j]);
            }
        }
        
        //chi
        for(int i = 0; i < 5; i++) {
            for(int j = 0; j < 5; j++) {
                A[i][j] = B[i][j] ^ ((~B[(i + 1) % 5][j]) & B[(i + 2) % 5][j]);
            }
        }

        //iota
        A[0][0] = A[0][0] ^ sha3_RC[k];
        // printf("round%d: A[0][0]:0x%016llx\n", k, A[0][0]);
    }
}

void SHA3_256(u8* in, u64** out, u64 bytenum) {
    u64 currbyte = padding(in, bytenum);
    // printf("current: %lld\n", currbyte);
    u64 blocknum = currbyte / 136;
    u64 A[5][5] = {};

    // for(int j = 0; j < 136; j++) {
    //     printf("in[%d]: 0x%02x\n", j, in[j]);
    // }

    for(int i = 0; i < blocknum; i++) {
        u64 block[17] = {};
        for(int j = 0; j < 136; j++) {
            block[j / 8] = (block[j / 8] >> 8) + (u64(in[i * 136 + j]) << 56);
        }
        // for(int j = 0; j < 17; j++) {
        //     printf("block[%d]: 0x%llx\n", j, block[j]);
        // }
        for(int j = 0; j < 5; j++) {
            for(int k = 0; k < 5 && 5 * j + k < 17; k++) {
                A[k][j] ^= block[5 * j + k];
                // printf("A[%d][%d]: 0x%016llx\n", k, j, A[k][j]);
            }
        }
        Keccak_p(A);
    }
    for(int i = 0; i < 5; i++) {
        for(int j = 0; j < 5; j++) {
            out[i][j] = A[i][j];
        }
    }
    // printf("fuck\n");
}

#ifdef DEBUG
void sha3_debug() {
    // u8 in[136] = {0x61, 0x62, 0x63};
    u8 in[136] = {};
    u64** out = new u64*[5];
    // u64 bytenum = 3;
    u64 bytenum = 0;
    for(int i = 0; i < 5; i++) {
        out[i] = new u64[5];
    }

    SHA3_256(in, out, bytenum);

    for(int j = 0; j < 4; j++) {
        printf("out[%d][0]: %016llx\n",  j, reverse(out[j][0]));
    }
    // for(int j = 0; j < 5; j++) {
    //     printf("out[0][%d]: %016llx\n",  j, out[0][j]);
    // }

}
#endif