#pragma once
#include "consts.h"

#ifdef DEBUG
#include <cstdio>
#endif

u64 sm3_padding(u8* in, u64 bytenum);
u32 ROL(u32 x, int n);
u32 T(int j);
u32 FF(u32 x, u32 y, u32 z, int j);
u32 GG(u32 x, u32 y, u32 z, int j);
u32 P0(u32 x);
u32 P1(u32 x);
void CF(u32* V, u32* block);
void SM3(u8* in, u32* out, u64 bytenum);

#ifdef DEBUG
void sm3_debug();
#endif