#ifndef METAL_H
#define METAL_H


#if defined(__APPLE__)
#include <mach/mach_time.h>
#endif

#if !defined(__APPLE__) && !defined(__linux__)
/* Native Windows (MSVC/MinGW) or any other target: no raw syscalls
   available, fall back to the portable C library exit(). */
#include <stdlib.h>
#endif

static inline void metal_exit(int status)
{
#if defined(__APPLE__) && defined(__aarch64__)
    __asm__ volatile(
        "mov w0, %w0\n"
        "mov x16, #1\n"
        "svc #0"
        :
        : "r"(status)
    );
#elif defined(__APPLE__) && defined(__x86_64__)
    __asm__ volatile(
        "movl %0, %%edi\n"
        "movl $0x2000001, %%eax\n"
        "syscall"
        :
        : "r"(status)
    );
#elif defined(__linux__) && defined(__x86_64__)
    __asm__ volatile(
        "movl %0, %%edi\n"
        "movl $60, %%eax\n"   /* sys_exit on Linux x86_64 */
        "syscall"
        :
        : "r"(status)
    );
#elif defined(__linux__) && defined(__aarch64__)
    __asm__ volatile(
        "mov w0, %w0\n"
        "mov x8, #93\n"       /* sys_exit on Linux aarch64 */
        "svc #0"
        :
        : "r"(status)
    );
#else
    /* Windows or unrecognized platform: portable fallback. */
    _exit(status);
#endif
}

static inline unsigned long long metal_get_ticks(void)
{
#if defined(__APPLE__)
    return mach_absolute_time();
#elif defined(__x86_64__)
    unsigned int lo, hi;
    __asm__ volatile("rdtsc" : "=a" (lo), "=d" (hi));
    return ((unsigned long long)hi << 32) | lo;
#elif defined(__aarch64__)
    unsigned long long val;
    __asm__ volatile("mrs %0, cntvct_el0" : "=r"(val));
    return val;
#else
    return 0;
#endif
}

static inline unsigned long long metal_get_freq(void)
{
#if defined(__APPLE__)
    mach_timebase_info_data_t tb;
    mach_timebase_info(&tb);
    return (1000000000ULL * tb.denom) / tb.numer;
#elif defined(__x86_64__)
    /* Approximate: TSC frequency is not architecturally fixed on x86,
       but this gives a stable-enough reference for relative timing. */
    return 2400000000ULL;
#elif defined(__aarch64__)
    unsigned long long freq;
    __asm__ volatile("mrs %0, cntfrq_el0" : "=r"(freq));
    return freq;
#else
    return 1;
#endif
}

#endif
