from chip8OpCodes import opCodes, wildCard

from pprint import pprint
from string import Template

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

def getSwitch(gen, default="", tree=op, tabdepth=0, opCodeDigit=0):
    out = ""
    tabs = '\t'*tabdepth
    if isinstance(tree, tuple):
        out += gen(tree)
    elif len(tree) == 1:
        out += getSwitch( gen, default, tree[tree.keys()[0]], tabdepth, opCodeDigit+1 )
    else:
        out += "switch (op.digit%s) {\n" % opCodeDigit

        for value, nest in sorted( tree.items() ):
            if value == 'X':
                continue
            else:
                out += tabs + "case 0x%s: " % value
                out += getSwitch( gen, default, tree[value], tabdepth+1, opCodeDigit+1 )
                out += " break;\n"

        out += tabs + "default: "
        if 'X' in tree.keys():
            out += getSwitch( gen, default, tree['X'], tabdepth+1, opCodeDigit+1 )
            out += tabs + " break;"
        else:
            out += default + " break;"

        out += "\n" + tabs + "}"
    return out

def genCode(tree):
    return tree[0] + " "

#print getSwitch(genCode)









