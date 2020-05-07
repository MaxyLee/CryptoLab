#include "rc4.h"
#include <cstring>

void RC4(u8* key, u8* in, u8* out, u64 keylen, u64 length, bool encrypt) {
    u8 S[256] = {};
    u8 T[256] = {};
    u8* key_stream = new u8[length];

    for(int i = 0; i < 256; i++) {
        S[i] = i;
        T[i] = key[i % keylen];
    }

    int index = 0;
    for(int i = 0; i < 256; i++) {
        index = (index + S[i] + T[i]) % 256;
        u8 tmp = S[index];
        S[index] = S[i];
        S[i] = tmp;
    }

    int i = 0;
    int j = 0;
    for(int r = 0; r < length; r++) {
        i = (i + 1) % 256;
        j = (j + S[i]) % 256;
        u8 tmp = S[j];
        S[j] = S[i];
        S[i] = tmp;
        int t = (S[i] + S[j]) % 256;
        key_stream[r] = S[t];
        out[r] = in[r] ^ key_stream[r];
    }
}

#ifdef DEBUG
void rc4_debug() {
    
}
#endif