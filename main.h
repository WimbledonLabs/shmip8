#include <stdint.h>

typedef uint8_t uint8;
typedef uint16_t uint16;

typedef union{
    uint16 value;
    struct {
        unsigned int : 4;
        unsigned int nNN: 12;
    };

    struct {
        unsigned int : 8;
        unsigned int nN: 8;
    };
    
    struct {
        unsigned int : 12;
        unsigned int n: 4;
    };
    
    struct {
        unsigned int : 8;
        unsigned int longCode: 8;
    };

    struct {
        unsigned int type: 4;
        unsigned int vX: 4;
        unsigned int vY: 4;
        unsigned int code: 4;
    };
} OpCode;

void execute(OpCode);
void fetch(OpCode);
