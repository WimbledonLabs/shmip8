import sys
from small_switch import getSwitch
from string import Template

def generateSimpleDisassemble(tree):
    d = {
        "vX": '" << std::hex << ((int) op.vX) << "',
        "vY": '" << std::hex << ((int) op.vY) << "',
        "n": '" << "#" << std::hex << ((int) op.n) << "',
        "nN": '" << "#" << std::hex << ((int) op.nN) << "',
        "nNN": '" << "#" << std::hex << ((int) op.nNN) << "',
    }

    return 'std::cout << "' + Template(tree[1]).substitute(d) + '";'

def generateStateInfo(tree):
    #set(['keys', 'vF', 'I', 'pc', 'ramUpToVX', 'vX', 'vY', 'dT', 'sT', 'regUpToVX', 'BCD'])
    d = {
        "vX": "std::hex << \"0x\"  << ((int) core->reg[op.vX])",
        "vY": "std::hex << \"0x\" << ((int) core->reg[op.vY])",
        "vF": "std::hex << \"0x\" << ((int) core->reg[0xF])",
        "I": "std::hex << \"0x\" << ((int) core->I)",
        "pc": "std::hex << \"0x\" << ((int) core->pc)",
        "regUpToVX": "regUpToVX(core, op)",
        "ramUpToVX": "ramUpToVX(core, op)",
        "keys": "core->keyboard.keys",
        "dT": "std::hex << \"0x\" << ((int) core->dT)",
        "sT": "std::hex << \"0x\" << ((int) core->sT)",
        "BCD": "BCD(core, op)",
    }

    if tree[2]:
        changeList = [ "\"%s: \" << %s" % (i, d[i]) for i in tree[2] ]
        return "std::cout << %s;" % " << \"  \" << ".join(changeList)
    else:
        return "";

def genCode(tree):
    return tree[0]


genFunc = { 
    "code":         (genCode,                   "/* opUnknown(); */"),
    "state":        (generateStateInfo,         "std::cout << \"NOP\";"),
    "disassemble":  (generateSimpleDisassemble, "std::cout << \"NOP\";")
}

if __name__ == '__main__':
    #print getSwitch(generateStateInfo, "std::cout << \"NOP\";")
    if sys.argv[1] in genFunc.keys():
        print getSwitch(genFunc[sys.argv[1]][0], genFunc[sys.argv[1]][1])
    else:
        raise ValueError("Arg must be in %s" % genFunc.keys())
