#include "shmip8_common.h"

typedef union{
    uint16 value;
    struct {
        unsigned int nNN: 12;
        unsigned int : 4;
    };

    struct {
        unsigned int nN: 8;
        unsigned int : 8;
    };
    
    struct {
        unsigned int n: 4;
        unsigned int : 12;
    };
    
    struct {
        unsigned int longCode: 8;
        unsigned int : 8;
    };

    struct {
        unsigned int code: 4;
        unsigned int vY: 4;
        unsigned int vX: 4;
        unsigned int type: 4;
    };
} OpCode;

void execute(OpCode);
void fetch(OpCode);
