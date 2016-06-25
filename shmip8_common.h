#ifndef SHMIP8_COMMON_H
#define SHMIP8_COMMON_H
#include <stdint.h>

typedef uint8_t uint8;
typedef uint16_t uint16;
typedef uint32_t uint32;

typedef union{
    uint16 keys = 0;
    struct {
        unsigned int kF: 1;
        unsigned int kE: 1;
        unsigned int kD: 1;
        unsigned int kC: 1;
        unsigned int kB: 1;
        unsigned int kA: 1;
        unsigned int k9: 1;
        unsigned int k8: 1;
        unsigned int k7: 1;
        unsigned int k6: 1;
        unsigned int k5: 1;
        unsigned int k4: 1;
        unsigned int k3: 1;
        unsigned int k2: 1;
        unsigned int k1: 1;
        unsigned int k0: 1;
    };
} HexKeyboardStatus;

#endif // SHMIP8_COMMON_H
