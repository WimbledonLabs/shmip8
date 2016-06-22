from pprint import pprint

opCodes = {
        "0NNN": "opCallRCA",
        "00E0": "opClearScreen",
        "00EE": "opReturnSubroutine",
        "1NNN": "opJump",
        "2NNN": "opCallSubroutine",
        "3XNN": "opSkipEq",
        "4XNN": "opSkipNeq",
        "5XY0": "opSkipREq",
        "6XNN": "opMovConstant",
        "7XNN": "opAddConstant",
        "8XY0": "opMov",
        "8XY1": "opOr",
        "8XY2": "opAnd", 
        "8XY3": "opXor",  
        "8XY4": "opAdd",
        "8XY5": "opSub",
        "8XY6": "opRShift",
        "8XY7": "opRSub",
        "8XYE": "opLShift",
        "9XY0": "opSkipRNeq",
        "ANNN": "opSetIndex",
        "BNNN": "opJumpOffset",
        "CXNN": "opRandAnd",
        "DXYN": "opDraw",
        "EX9E": "opSkipWhenKeyDown",
        "EXA1": "opSkipWhenKeyUp",
        "FX07": "opGetDelayTimer",
        "FX0A": "opWaitForKeyPress",
        "FX15": "opSetDelayTimer",
        "FX18": "opGetSoundTimer",
        "FX1E": "opAddToIndex",
        "FX29": "opSetIndexToCh",
        "FX33": "opConvertToBCD",
        "FX55": "opPushRange",
        "FX65": "opPopRange"
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

def getSwitch(tree, tabdepth=0, opCodeDigit=0):
    out = ""
    tabs = '\t'*tabdepth
    if isinstance(tree, basestring):
        out += tabs + tree + "();\n"
    elif len(tree) == 1:
        out += getSwitch( tree[tree.keys()[0]], tabdepth, opCodeDigit+1 )
    else:
        out += tabs + "select (opCode.digit%s) {\n" % opCodeDigit

        for value, nest in sorted( tree.items() ):
            if value == 'X':
                continue
            else:
                out += tabs + "case 0x%s:\n" % value
                out += getSwitch( tree[value], tabdepth+1, opCodeDigit+1 )
                out += tabs + "\tbreak;\n"

        out += tabs + "default:\n"
        if 'X' in tree.keys():
            out += getSwitch( tree['X'], tabdepth+1, opCodeDigit+1 )
        else:
            out += tabs + "opUnknown();"
        out += tabs + "\tbreak;\n"

        out += tabs + "}\n"
    return out

print getSwitch(op)
print sorted( opCodes.keys() )
