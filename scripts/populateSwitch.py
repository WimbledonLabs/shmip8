import argparse
import small_switch

parser = argparse.ArgumentParser(description="Add the opcode switch statement into source file")
parser.add_argument('source', 
        help='File which includes the line "#define OPCODE_SWITCH_START"')
parser.add_argument('dest', help="The output file")

args = parser.parse_args();

src = open(args.source)
dest = open(args.dest, 'w')

for line in src.readlines():
    dest.write(line)
    if "#define OPCODE_SWITCH_START" in line:
        dest.write("// Generated using %s\n" % __file__)
        dest.write(small_switch.getSwitch() + '\n')

src.close()
dest.close()
