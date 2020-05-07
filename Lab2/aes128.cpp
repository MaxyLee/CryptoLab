#include "aes128.h"
#include <cstring>

u32 w[44] = {};

u32 T(u32 w, int round) {
    u32 shift = (w << 8) + (w >> 24);
    u32 res = 0;
    for(int i = 0; i < 4; i++) {
        res = (res << 8) + sBox[(shift >> (24 - 8 * i)) & 0xff];
    }
    return res ^ RC[round];
}


u8 XTIME(u8 x) {
	return ((x << 1) ^ ((x & 0x80) ? 0x1b : 0x00));
}

u8 GFMul(u8 a, u8 b) {
	u8 temp[8] = {a};
	u8 tempmultiply = 0x00;
	int i = 0;
	for (i = 1; i < 8; i++) {
		temp[i] = XTIME(temp[i - 1]);
	}
	tempmultiply = (b & 0x01) * a;
	for (i = 1; i <= 7; i++) {
		tempmultiply ^= (((b >> i) & 0x01) * temp[i]);
	}
	return tempmultiply;
}

void gen_subkeys(u8 key[]) {
    for(int i = 0; i < 4; i++) {
        for(int j = 0; j < 4; j++) {
            w[i] = (w[i] << 8) + key[4 * j + i];
        }
    }

    for(int i = 4; i < 44; i++) {
        if(i % 4 == 0) {
            w[i] = w[i - 4] ^ T(w[i - 1], i / 4 - 1);
        } else {
            w[i] = w[i - 4] ^ w[i - 1];
        }
    }
}

void add_roundkey(u8* in, u8* out, int round) {
    for(int i = 0; i < 4; i++) {
        for(int j = 0; j < 4; j++) {
            out[4 * i + j] = in[4 * i + j] ^ u8(w[4 * round + j] >> (24 - 8 * i));
        }
    }
}

void sub_bytes(u8* in, u8* out) {
    for(int i = 0; i < 4; i++) {
        for(int j = 0; j < 4; j++) {
            out[4 * i + j] = sBox[in[4 * i + j]];
        }
    }
}

void inv_sub_bytes(u8* in, u8* out) {
    for(int i = 0; i < 4; i++) {
        for(int j = 0; j < 4; j++) {
            out[4 * i + j] = invsBox[in[4 * i + j]];
        }
    }
}

void shift_row(u8* in, u8* out) {
    for(int i = 0; i < 4; i++) {
        for(int j = 0; j < 4; j++) {
            out[4 * i + j] = in[4 * i + (j + i) % 4];
        }
    }
}

void inv_shift_row(u8* in, u8* out) {
    for(int i = 0; i < 4; i++) {
        for(int j = 0; j < 4; j++) {
            out[4 * i + j] = in[4 * i + (j - i + 4) % 4];
        }
    }
}

void mix_col(u8* in, u8* out) {
    for(int i = 0; i < 4; i++) {
        for(int j = 0; j < 4; j++) {
            out[4 * i + j] = 0;
            for(int k = 0; k < 4; k++) {
                out[4 * i + j] = out[4 * i + j] ^ GFMul(Mcol[i][k], in[4 * k + j]);
            }
        }
    }
}

void inv_mix_col(u8* in, u8* out) {
    for(int i = 0; i < 4; i++) {
        for(int j = 0; j < 4; j++) {
            out[4 * i + j] = 0;
            for(int k = 0; k < 4; k++) {
                out[4 * i + j] = out[4 * i + j] ^ GFMul(invMcol[i][k], in[4 * k + j]);
            }
        }
    }
}

void AES128(u8* in, u8* out, bool encrypt) {
    u8 add[16] = {};
    u8 sub[16] = {};
    u8 shift[16] = {};
    u8 mix[16] = {};
    if(encrypt) {
        add_roundkey(in, add, 0);
        
        // printf("add: 0x");
        // for(int i = 0; i < 16; i++) {
        //     printf("%x", add[i]);
        // }
        // printf("\n");

        for(int i = 1; i < 10; i++) {
            sub_bytes(add, sub);
            #ifdef DEBUG
            printf("sub[%d]: \n", i);
            for(int j = 0; j < 16; j++) {
                printf("%02x", sub[j]);
                if(j % 4 == 3) {
                    printf("\n");
                }
            }
            printf("\n");
            #endif
            shift_row(sub, shift);
            #ifdef DEBUG
            printf("shift[%d]: \n", i);
            for(int j = 0; j < 16; j++) {
                printf("%02x", shift[j]);
                if(j % 4 == 3) {
                    printf("\n");
                }
            }
            printf("\n");
            #endif
            mix_col(shift, mix);
            #ifdef DEBUG
            printf("mix[%d]: \n", i);
            for(int j = 0; j < 16; j++) {
                printf("%02x", mix[j]);
                if(j % 4 == 3) {
                    printf("\n");
                }
            }
            printf("\n");
            #endif
            add_roundkey(mix, add, i);
            #ifdef DEBUG
            printf("add[%d]: \n", i);
            for(int j = 0; j < 16; j++) {
                printf("%02x", add[j]);
                if(j % 4 == 3) {
                    printf("\n");
                }
            }
            printf("**********************\n");
            #endif
        }
        //round 10
        sub_bytes(add, sub);
        #ifdef DEBUG
        printf("sub[10]: \n");
        for(int j = 0; j < 16; j++) {
            printf("%02x", sub[j]);
            if(j % 4 == 3) {
                printf("\n");
            }
        }
        printf("\n");
        #endif
        shift_row(sub, shift);
        #ifdef DEBUG
        printf("shift[10]: \n");
        for(int j = 0; j < 16; j++) {
            printf("%02x", shift[j]);
            if(j % 4 == 3) {
                printf("\n");
            }
        }
        printf("\n");
        #endif
        add_roundkey(shift, out, 10);
        #ifdef DEBUG
        printf("add[10]: \n");
        for(int j = 0; j < 16; j++) {
            printf("%02x", add[j]);
            if(j % 4 == 3) {
                printf("\n");
            }
        }
        printf("**********************\n");
        #endif
    } else {
        add_roundkey(in, mix, 10);
        for(int i = 9 ; i > 0; i--) {
            inv_shift_row(mix, shift);
            inv_sub_bytes(shift, sub);
            add_roundkey(sub, add, i);
            inv_mix_col(add, mix);
        }
        //round 10
        inv_shift_row(mix, shift);
        inv_sub_bytes(shift, sub);
        add_roundkey(sub, out, 0);
    }
}

void AES128_CBC(u8 iv[], u8 key[], u8* in, u8* out, u64 length, bool encrypt) {
    u8 tmp[16];
    memcpy(tmp, iv, sizeof(tmp));
    gen_subkeys(key);
    if(encrypt) {
        for(int i = 0; i < length; i++) {
            for(int j = 0; j < 16; j++) {
                tmp[j] = tmp[j] ^ in[i * 16 + j];
            }
            AES128(tmp, out + i * 16, true);
            for(int j = 0; j < 16; j++) {
                tmp[j] = out[i * 16 + j];
            }
        } 
    } else {
        for(int i = 0; i < length; i++) {
            AES128(in + i * 16, out + i * 16, false);
            for(int j = 0; j < 16; j++) {
                out[i * 16 + j] = out[i * 16 + j] ^ tmp[j];
                tmp[j] = in[i * 16 + j];
            }
        } 
    }
    
}

#ifdef DEBUG
void aes128_debug() {
    u8 plain[16] = {0x68, 0x65, 0x6c, 0x6c, 0x6f, 0x20, 0x64, 0x61, 0x6e, 0x69, 0x65, 0x6c, 0x20, 0x74, 0x6f, 0x20};
    // u8 plain[16] ={
    //     0x32,0x88,0x31,0xe0,
    //     0x43,0x5a,0x31,0x37,
    //     0xf6,0x30,0x98,0x07,
    //     0xa8,0x8d,0xa2,0x34
    // };
    u8 key[16] = {0x31, 0x31, 0x31, 0x31, 0x31, 0x31, 0x31, 0x31, 0x31, 0x31, 0x31, 0x31, 0x31, 0x31, 0x31, 0x31};
    // u8 key[16] = {
    //     0x2b,0x28,0xab,0x09,
    //     0x7e,0xae,0xf7,0xcf,
    //     0x15,0xd2,0x15,0x4f,
    //     0x16,0xa6,0x88,0x3c
    // };
    // u8 key[16] = {0x3C, 0xA1, 0x0B, 0x21, 0x57, 0xF0, 0x19, 0x16, 0x90, 0x2E, 0x13, 0x80, 0xAC, 0xC1, 0x07, 0xBD};
    u8 cipher[16] = {};
    gen_subkeys(key);
    // for(int i = 0; i < 44; i++) {
    //     printf("W[%d]: 0x%08x\n", i, w[i]);
    // }
    AES128(plain, cipher, true);
    printf("cipher: 0x");
    for(int i = 0; i < 16; i++) {
        printf("%x", cipher[i]);
    }
    printf("\n");
}
#endif