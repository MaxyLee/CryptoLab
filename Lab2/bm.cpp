#include "bm.h"
#include <cstring>

void B_M(u8* a, u64 N) {
    u8** f = new u8*[N + 1];
    u8* d = new u8[N];
    u8* l = new u8[N + 1];
    memset(d, 0, N);
    memset(l, 0, N);
    for(int i = 0; i <= N; i++) {
        f[i] = new u8[N];
        memset(f[i], 0, N);
    }

    f[0][0] = 1;
    int m = -1;
    for(int i = 0; i < N; i++) {
        for(int j = 0; j <= l[i]; j++) {
            d[i] += f[i][j] * a[i - j];
        }
        d[i] = d[i] % 2;
        if(i > 0 && l[i] > l[i - 1]) {
            m = i - 1;
        }
        if(d[i] == 0) {
            memcpy(f[i + 1], f[i], N);
            l[i + 1] = l[i];
        } else {
            if(m == -1) {
                f[i + 1][0] = 1;
                f[i + 1][i + 1] = 1;
                l[i + 1] = i + 1;
            } else {
                memcpy(f[i + 1], f[i], N);
                for(int j = 0; j < N - i + m; j++) {
                    f[i + 1][j + i - m] += f[m][j];
                    f[i + 1][j + i - m] %= 2;
                }
                l[i + 1] = l[i] > i + 1 - l[i] ? l[i] : i + 1 - l[i];
            }
        }
        
        #ifdef DEBUG
        printf("f_%d(x) = ", i);
        if(f[i][0] > 0) {
            printf("1 ");
        }
        for(int j = 1; j < N; j++) {
            if(f[i][j] > 0) {
                printf("+ %ux^%d ", f[i][j], j);
            }
        }
        printf("\n");
        printf("l[%u] = %u\n", i, l[i]);
        printf("d[%d] = %u\n", i, d[i]);
        printf("m = %d\n", m);
        printf("*******************\n");
        #endif
        
    }
    #ifdef DEBUG
    printf("f_%d(x) = ", N);
    if(f[N][0] > 0) {
            printf("1 ");
        }
        for(int j = 1; j < N; j++) {
            if(f[N][j] > 0) {
                printf("+ %ux^%d ", f[N][j], j);
            }
        }
    printf("\n");
    printf("l[%u] = %u\n", N, l[N]);
    #endif
}

#ifdef DEBUG
void bm_debug() {
    //0010101001
    u8 a[] = {0, 0, 1, 0, 1, 0, 1, 0, 0, 1};
    u64 N = 10;
    B_M(a, N);
}
#endif