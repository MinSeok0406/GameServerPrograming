#pragma once

void* alloc_overflow_check(int size);
void  free_overflow_check(void* ptr, int size);