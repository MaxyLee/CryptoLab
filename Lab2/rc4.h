#pragma once
#include "consts.h"

#ifdef DEBUG
#include <cstdio>
#endif

void RC4(u8* key, u8* in, u8* out, u64 keylen, u64 length);

#ifdef DEBUG
void rc4_debug();
#endif