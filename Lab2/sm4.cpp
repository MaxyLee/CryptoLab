#include "sm4.h"
#include <cstring>

u32 rk[32] = {};

void gen_rk(u32* MK, bool encrypt) {
    u32 K[4] = {};
    for(int i = 0; i < 4; i++) {
        K[i] = MK[i] ^ FK[i];
    }

    for(int i = 0; i < 32; i++) {
        u32 t = tau(K[1] ^ K[2] ^ K[3] ^ CK[i]);
        u32 l = t ^ ((t << 13) | (t >> 19)) ^ ((t << 23) | (t >> 9));
        rk[i] = K[0] ^ l;
        for(int j = 0; j < 3; j++) {
            K[j] = K[j + 1];
        }
        K[3] = rk[i];
    }
    if(!encrypt) {
        for(int i = 0; i < 16; i++) {
            u32 tmp = rk[i];
            rk[i] = rk[31 - i];
            rk[31 - i] = tmp;
        }
    }

}

u32 tau(u32 in) {
    u32 out = 0;
    for(int i = 0; i < 4; i++) {
        out = (out << 8) + (sm4_SBox[u8(in >> (24 - 8 * i))]);
    }
    return out;
}

void SM4(u32* in, u32* out) {
    u32 X[4] = {};
    for(int i = 0; i < 4; i++) {
        X[i] = in[i];
    }
    for(int i = 0; i < 32; i++) {
        u32 t = tau(X[1] ^ X[2] ^ X[3] ^ rk[i]);
        u32 l = t ^ ((t << 2) | (t >> 30)) ^ ((t << 10) | (t >> 22)) ^ ((t << 18) | (t >> 14)) ^ ((t << 24) | (t >> 8));
        l = X[0] ^ l;
        for(int j = 0; j < 3; j++) {
            X[j] = X[j + 1];
        }
        X[3] = l;
        // printf("X[%d]:0x%x\n", i, l);
    }
    for(int i = 0; i < 4; i++) {
        out[i] = X[3 - i];
    }
}

void SM4_CBC(u32 iv[], u32 MK[], u32* in, u32* out, u64 length, bool encrypt) {
    u32 tmp[4] = {};
    memcpy(tmp, iv, sizeof(tmp));
    gen_rk(MK, encrypt);
    if(encrypt) {
        for(int i = 0; i < length; i++) {
            for(int j = 0; j < 4; j++) {
                tmp[j] = tmp[j] ^ in[i * 4 + j];
            }
            SM4(tmp, out + i * 4);
            for(int j = 0; j < 4; j++) {
                tmp[j] = out[i * 4 + j];
            }
        }
    } else {
        for(int i = 0; i < length; i++) {
            SM4(in + i * 4, out + i * 4);
            for(int j = 0; j < 4; j++) {
                out[i * 4 + j] = out[i * 4 + j] ^ tmp[j];
                tmp[j] = out[i * 4 + j];
            }
        }
    }
}

#ifdef DEBUG
void sm4_debug() {
    u32 MK[4] = {0x01234567, 0x89abcdef, 0xfedcba98, 0x76543210};
    u32 in[4] = {0x01234567, 0x89abcdef, 0xfedcba98, 0x76543210};
    u32 out[4] = {};
    gen_rk(MK, true);
    for(int i = 0; i < 32; i++) {
        printf("rk[%d]:0x%x\n", i, rk[i]);
    }
    SM4(in, out);
    for(int i = 0; i < 4; i++) {
        printf("out[%d]:0x%x\n", i, out[i]);
    }
}
#endif

