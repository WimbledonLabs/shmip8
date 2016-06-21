from pprint import pprint

opCodes = {
        "0NNN": "callRCA",
        "00E0": "clearScreen",
        "00EE": "returnSubroutine",
        "1NNN": "jump",
        "2NNN": "callSubroutine",
        "3XNN": "skipEq",
        "4XNN": "skipNeq",
        "5XY0": "skipREq",
        "6XNN": "setReg",
        "7XNN": "addReg",
        "8XY0": "movReg",
        "8XY1": "orReg",
        "8XY2": "andReg", 
        "8XY3": "xorReg",  
        "8XY4": "addFReg",
        "8XY5": "subFReg",
        "8XY6": "rshFReg",
        "8XY7": "rsubFReg",
        "8XYE": "lshFReg",
        "9XY0": "skipRNeq",
        "ANNN": "setIndex",
        "BNNN": "jumpOffset",
        "CXNN": "randAnd",
        "DXYN": "draw",
        "EX9E": "skipWithKey",
        "EXA1": "skipNotKey",
        "FX07": "getDelayTimer",
        "FX0A": "waitForKey",
        "FX15": "setDelayTimer",
        "FX18": "setSoundTimer",
        "FX1E": "addIndex",
        "FX29": "setIndexToCh",
        "FX33": "convertBCD",
        "FX55": "pushRange",
        "FX65": "popRange"
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
    if isinstance(tree, basestring):
        out += '\t'*tabdepth + tree + "();\n"
    elif len(tree) == 1:
        out += getSwitch( tree[tree.keys()[0]], tabdepth, opCodeDigit+1 )
    else:
        tabs = '\t'*tabdepth
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
        out += tabs + "\tbreak;\n"

        out += tabs + "}\n"
    return out

print getSwitch(op)
print sorted( opCodes.keys() )
