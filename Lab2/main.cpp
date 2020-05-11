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
#include "sm3.h"

int main(int argc, char* argv[]) {
    srand(time(0));
    clock_t start, end;
    char filein[] = "random.in";
    
    #ifdef DEBUG
    sm3_debug();
    // sha3_debug();
    // sha2_debug();
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

        printf("iv: 0x%016llx\n", iv);
        printf("key: 0x%016llx\n", key);

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

        FILE* fout = fopen("DES.out", "w");
        fwrite(out, sizeof(out), 256, fout);
        fclose(fout);
    } else if(strcmp(argv[1], "AES") == 0) {
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

        printf("iv: 0x");
        for(int i = 0; i < 16; i++) {
            printf("%02x", iv[i]);
        }
        printf("\n");
        printf("key: 0x");
        for(int i = 0; i < 16; i++) {
            printf("%02x", key[i]);
        }
        printf("\n");

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

        FILE* fout = fopen("AES.out", "w");
        fwrite(out, sizeof(out), 2048, fout);
        fclose(fout);
    } else if(strcmp(argv[1], "SM4") == 0) {
        u32 iv[4] = {};
        u32 MK[4] = {};
        u32 in[512] = {};
        u32 out[512] = {};
        FILE* fin = fopen(filein, "r");
        char buff[32];
        u64 length = 0;
        u64 cnt = 0;

        for(int i = 0; i < 4; i++) {
            iv[i] = rand();
            MK[i] = rand();
        }

        printf("iv: 0x");
        for(int i = 0; i < 4; i++) {
            printf("%08x", iv[i]);
        }
        printf("\n");
        printf("key: 0x");
        for(int i = 0; i < 4; i++) {
            printf("%08x", MK[i]);
        }
        printf("\n");

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

        FILE* fout = fopen("SM4.out", "w");
        fwrite(out, sizeof(out), 512, fout);
        fclose(fout);
    } else if(strcmp(argv[1], "RC4") == 0) {
        u8 key[256] = {};
        u8 in[2048] = {};
        u8 out[2048] = {};
        FILE* fin = fopen(filein, "r");
        char buff[8];
        u64 length = 0;
        u64 keylen = 256;
        for(int i = 0; i < keylen; i++) {
            key[i] = u8(rand());
        }

        printf("key: 0x");
        for(int i = 0; i < keylen; i++) {
            printf("%02x", key[i]);
        }
        printf("\n");

        while(fgets(buff, 9, fin) != NULL) {
            for(int i = 0; i < 8; i++) {
                in[length] = (in[length] << 1) + u8(buff[i] - '0');
            }
            length++;
        }
        fclose(fin);

        start = clock();
        RC4(key, in, out, keylen, length);
        end = clock();

        FILE* fout = fopen("RC4.out", "w");
        fwrite(out, sizeof(out), 2048, fout);
        fclose(fout);
    } else if(strcmp(argv[1], "BM") == 0) {
        u8 a[10] = {};
        u64 N = 10;
        u8 out[11] = {};
        for(int i = 0; i < N; i++) {
            a[i] = rand() % 2;
        }

        start = clock();
        B_M(a, out, N);
        end = clock();

        printf("res: <");
        for(int i = 0; i < N; i++) {
            printf("%u", out[i]);
        }
        printf(", %u>\n", out[N]);
    }else if(strcmp(argv[1], "SHA2") == 0) {
        u32 in[512] = {};
        u32 out[8] = {};
        FILE* fin = fopen(filein, "r");
        char buff[32];
        u64 bytenum = 0;

        while(fgets(buff, 33, fin) != NULL) {
            for(int i = 0; i < 32; i++) {
                in[bytenum] = (in[bytenum] << 1) + u32(buff[i] - '0');
            }
            bytenum++;
        }
        fclose(fin);

        start = clock();
        SHA2_256(in, out, bytenum);
        end = clock();

        printf("res: 0x");
        for(int i = 0; i < 8; i++) {
            printf("%08x", out[i]);
        }
        printf("\n");
    } else if(strcmp(argv[1], "SHA3") == 0) {
        u8 in[2112] = {};
        u64** out = new u64*[5];
        FILE* fin = fopen(filein, "r");
        char buff[8];
        u64 bytenum = 0;
        for(int i = 0; i < 5; i++) {
            out[i] = new u64[5];
        }

        while(fgets(buff, 9, fin) != NULL) {
            for(int i = 0; i < 8; i++) {
                in[bytenum] = (in[bytenum] << 1) + u8(buff[i] - '0');
            }
            bytenum++;
        }
        fclose(fin);

        start = clock();
        SHA3_256(in, out, bytenum);
        end = clock();

        printf("res: 0x");
        for(int j = 0; j < 4; j++) {
            printf("%016llx", reverse(out[j][0]));
        }
        printf("\n");

        for(int i = 0; i < 5; i++) {
            delete []out[i];
        }
        delete []out;
    } else if(strcmp(argv[1], "SM3") == 0) {
        u8 in[2112] = {};
        u32 out[8] = {};
        FILE* fin = fopen(filein, "r");
        char buff[8];
        u64 bytenum = 0;

        while(fgets(buff, 9, fin) != NULL) {
            for(int i = 0; i < 8; i++) {
                in[bytenum] = (in[bytenum] << 1) + u8(buff[i] - '0');
            }
            bytenum++;
        }
        fclose(fin);

        start = clock();
        SM3(in, out, bytenum);
        end = clock();

        printf("res: 0x");
        for(int i = 0; i < 8; i++) {
            printf("%08x", out[i]);
        }
        printf("\n");
    }  else {
        printf("no such method: %s\n", argv[1]);
        return 0;
    }
    double time = double(end - start) / CLOCKS_PER_SEC;
    double bandwidth = (1.0 / (1 << 6)) / time;
    if(strcmp(argv[1], "BM") == 0) {
        bandwidth = (0.625 / (1 << 10)) / time;
    }
    printf("time used: %lfsecs\n", time);
    printf("bandwidth: %llfMbps\n", bandwidth);

    #endif
    return 0;
}