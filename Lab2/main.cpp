#include <cstdio>
#include "des.h"

int main() {
    u64 key, text;
    scanf("%llu\n%lly\n", &key, &text);
    printf("encrypt %lld = %lld\n", text, DES(key, text, true));
    return 0;
}