#include <cstdio>
#include <cstdlib>
#include <ctime>
#include <cstring>
#include "des.h"
#include "aes128.h"
#include "sm4.h"
#include "rc4.h"
#include "bm.h"
#include "sha2.h"
#include "sha3.h"

int main(int argc, char* argv[]) {
    srand(time(0));
    clock_t start, end;
    char filein[] = "random.in";
    
    #ifdef DEBUG
    sha2_debug();
    // bm_debug();
    // sm4_debug();
    // aes128_debug();
    // des_debug();
    #else
    printf("using %s:\n", argv[1]);
    if(strcmp(argv[1], "DES") == 0) {
        u64 iv = u64(rand()) + (u64(rand()) << 32);
        u64 key = u64(rand()) + (u64(rand()) << 32);
        u64 in[256] = {};
        u64 out[256] = {};
        FILE* fin = fopen(filein, "r");
        char buff[64];
        u64 length = 0;

        printf("iv: 0x%llx\n", iv);
        printf("key: 0x%llx\n", key);

        while(fgets(buff, 65, fin) != NULL) {
            for(int i = 0; i < 64; i++) {
                in[length] = (in[length] << 1) + (buff[i] - '0');
            }
            length++;
        }
        fclose(fin);

        start = clock();
        DES_CBC(iv, key, in, out, length, true);
        end = clock();
    } else if(strcmp(argv[1], "AES128") == 0) {
        u8 iv[16] = {};
        u8 key[16] = {};
        u8 in[2048] = {};
        u8 out[2048] = {};
        FILE* fin = fopen(filein, "r");
        char buff[8];
        u64 length = 0;
        u64 cnt = 0;

        for(int i = 0; i < 16; i++) {
            iv[i] = u8(rand());
            key[i] = u8(rand());
        }

        while(fgets(buff, 9, fin) != NULL) {
            for(int i = 0; i < 8; i++) {
                in[cnt] = (in[cnt] << 1) + u8(buff[i] - '0');
            }
            cnt++;
        }
        fclose(fin);

        length = cnt / 16;

        start = clock();
        AES128_CBC(iv, key, in, out, length, true);
        end = clock();
    } else if(strcmp(argv[1], "SM4") == 0) {
        u32 iv[4] = {};
        u32 MK[4] = {};
        u32 in[512] = {};
        u32 out[512] = {};
        FILE* fin = fopen(filein, "r");
        char buff[32];
        u64 length = 0;
        u64 cnt = 0;

        for(int i = 0; i < 16; i++) {
            iv[i] = rand();
            MK[i] = rand();
        }

        while(fgets(buff, 33, fin) != NULL) {
            for(int i = 0; i < 32; i++) {
                in[cnt] = (in[cnt] << 1) + u32(buff[i] - '0');
            }
            cnt++;
        }
        fclose(fin);

        length = cnt / 4;

        start = clock();
        SM4_CBC(iv, MK, in, out, length, true);
        end = clock();
    } else {
        printf("no such method: %s\n", argv[1]);
    }
    double time = double(end - start)/CLOCKS_PER_SEC;
    double bandwidth = (1.0 / (1 << 6)) / time;
    printf("time used: %lf\n", time);
    printf("bandwidth: %llfMbps\n", bandwidth);

    #endif
    return 0;
}