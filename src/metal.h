#ifndef METAL_H
#define METAL_H

#if defined(__aarch64__)
#include <mach/mach_time.h>
#endif

static inline void metal_exit(int status) 
{
#if defined(__aarch64__)
    __asm__ volatile(
        "mov w0, %w0\n"
        "mov x16, #1\n"
        "svc #0"
        :
        : "r"(status)
    );
#elif defined(__x86_64__)
    __asm__ volatile(
        "movl %0, %%edi\n"
        "movl $0x2000001, %%eax\n"
        "syscall"
        :
        : "r"(status)
    );
#endif
}

static inline unsigned long long metal_get_ticks(void) 
{
#if defined(__aarch64__)
    return mach_absolute_time();
#elif defined(__x86_64__)
    unsigned int lo, hi;
    __asm__ volatile("rdtsc" : "=a" (lo), "=d" (hi));
    return ((unsigned long long)hi << 32) | lo;
#else
    return 0;
#endif
}

static inline unsigned long long metal_get_freq(void) 
{
#if defined(__aarch64__)
    mach_timebase_info_data_t tb;
    mach_timebase_info(&tb);
    return (1000000000ULL * tb.denom) / tb.numer;
#elif defined(__x86_64__)
    return 2400000000ULL; 
#else
    return 1;
#endif
}

#endif