#pragma once
#include "consts.h"

#ifdef DEBUG
#include <cstdio>
#endif

void B_M(u8* a, u8* out, u64 N);

#ifdef DEBUG
void bm_debug();
#endif