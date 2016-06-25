from pprint import pprint
from string import Template

opCodes = {
    "0NNN": ("/* Do nothing */", "RCA ${nNN}"),
    "00E0": ("for(int xx=0; xx<CHIP8_SCREEN_WIDTH; xx++) "
                "for(int yy=0; yy<CHIP8_SCREEN_HEIGHT; yy++) "
                    "pixels[xx + yy * CHIP8_SCREEN_HEIGHT] = 0xFF << 24; "
                    "updateScreen = 1;", "CLR"),
    "00EE": ("pc = stack[sp--];", "RET"),
    "1NNN": ("pc = op.nNN;", "JP ${nNN}"),
    "2NNN": ("stack[++sp] = pc;", "CALL ${nNN}"),
    "3XNN": ("if(*x == op.nN) "
                "pc += 2;", "SE V${vX}, ${nN}"),
    "4XNN": ("if(*x != op.nN) "
                "pc += 2;", "SNE V${vX}, ${nN}"),
    "5XY0": ("if(*x == *y) "
                "pc += 2;", "SE V${vX}, V${vY}"),
    "6XNN": ("*x = op.nN;", "MOV V${vX}, ${nN}"),
    "7XNN": ("*x += op.nN;", "ADD V${vX}, ${nN}"),
    "8XY0": ("*x = *y;", "MOV V${vX}, V${vY}"),
    "8XY1": ("*x |= *y;", "OR V${vX}, V${vY}"),
    "8XY2": ("*x &= *y;", "AND V${vX}, V${vY}"),
    "8XY3": ("*x ^= *y;", "XOR V${vX}, V${vY}"),
    "8XY4": ("{uint16 p = *x + *y; "
            "reg[0xF] = p >> 8; "
            "*x = p;} ", "ADD V${vX}, V${vY}"),
    "8XY5": ("reg[0xF] = (*x >= *y) ? 1 : 0; "
            "*x -= *y;", "SUB V${vX}, V${vY}"),
    "8XY6": ("reg[0xF] = 0x1 & *x; "
            "*x >>= 1;", "RSH V${vX}"),
    "8XY7": ("reg[0xF] = (*x <= *y) ? 1 : 0; "
            "*x = *y - *x;", "RSUB V${vX}, V${vY}"),
    "8XYE": ("reg[0xF] = *x >> 7; *x <<= 1;", "LSH ${vX}"),
    "9XY0": ("if(*x != *y) pc += 2;", "SNE V${vX}, V${vY}"),
    "ANNN": ("I = op.nNN;", "MOV I, V${nNN}"),
    "BNNN": ("pc = reg[0] + op.nNN;", "B V${vX}, ${nNN}"),
    "CXNN": ("*x = rand() & op.nN;", "RAND V${vX}, ${nN}"),
    "DXYN": ("opDraw(op);", "DRW V${vX}, V${vY}, ${nN}"),
    "EX9E": ("if(  (0x8000 >> *x) && keyboard.keys)  pc += 2;", "SKP V${vX}"),
    "EXA1": ("if(!((0x8000 >> *x) && keyboard.keys)) pc += 2;", "SKNP V${vX}"),
    "FX07": ("*x = dT;", "MOV V${vX}, DT"),
    "FX0A": ("waitingForInput = 1; waitingInputReg = op.vX;", "WAIT V${vX}"),
    "FX15": ("dT = *x;", "MOV DT, V${vX}"),
    "FX18": ("sT = *x;", "MOV ST, V${vX}"),
    "FX1E": ("I += *x;", "ADD I, V${vX}"),
    "FX29": ("I = (*x)*FONT_HEIGHT + FONT_OFFSET;", "MOV F, V${vX}"),
    "FX33": ("ram[I]   =  *x / 100; "
            "ram[I+1] = (*x % 100) / 10; " 
            "ram[I+2] =  *x % 10;", "BCD V${vX}"),
    "FX55": ("for (int j=0; j<=op.vX; j++) "
                "ram[I++] = reg[j];", "PUSH V${vX}"),
    "FX65": ("for (int j=0; j<=op.vX; j++) "
                "reg[j] = ram[I++];", "POP V${vX}"),
}

branches = set("0123456789ABCDEF")
wildCard = "XYN"
op = {}

#TODO propagate callRCA (0NNN) into the other branches (00E0, 00EE)

for match, opName in opCodes.items():
    obj = op
    for i in range(0, len(match)):
        c = match[i]
        if c in wildCard:
            c = 'X'
        if i == len(match)-1:
            obj = obj.setdefault( c, opName )
        else:
            obj = obj.setdefault( c, {} );

def getSwitch(tree=op, tabdepth=0, opCodeDigit=0):
    out = ""
    tabs = '\t'*tabdepth
    if isinstance(tree, tuple):
        out += tree[0] + " "
    elif len(tree) == 1:
        out += getSwitch( tree[tree.keys()[0]], tabdepth, opCodeDigit+1 )
    else:
        out += "switch (opCode.digit%s) {\n" % opCodeDigit

        for value, nest in sorted( tree.items() ):
            if value == 'X':
                continue
            else:
                out += tabs + "case 0x%s: " % value
                out += getSwitch( tree[value], tabdepth+1, opCodeDigit+1 )
                out += "break;\n"

        out += tabs + "default: "
        if 'X' in tree.keys():
            out += getSwitch( tree['X'], tabdepth+1, opCodeDigit+1 )
            out += tabs + "break;"
        else:
            out += "/* opUnknown(); */ break;"

        out += "\n" + tabs + "} "
    return out

def getASM():
    out = ""
    names = { "vX": "X",
              "vY": "Y",
              "n": "N",
              "nN": "NN",
              "nNN": "NNN"}
    for opCode in sorted(opCodes.keys()):
        instr = Template(opCodes[opCode][1]).substitute(names).split(None, 1)
        if len(instr) == 1:
            instr.append(' ')
        out += "%s: %s\n" % (opCode, instr[0] + ' '*(5-len(instr[0])) + instr[1] )

    return out;

if __name__ == '__main__':
    print getASM();
