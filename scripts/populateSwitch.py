import argparse
import small_switch

parser = argparse.ArgumentParser(description="Add the opcode switch statement into source file")
parser.add_argument('-s', '--source', required=False,
        help='File which includes the line "#define OPCODE_SWITCH_START"')
parser.add_argument('dest', help="The output file")

args = parser.parse_args();

if args.source:
    src = open(args.source)
else:
    src = None
dest = open(args.dest, 'w')

if src:
    for line in src.readlines():
        dest.write(line)
        if "#define OPCODE_SWITCH_START" in line:
            dest.write("// Generated using %s\n" % __file__)
            dest.write(small_switch.getSwitch() + '\n')
else:
    dest.write("// Generated using %s\n" % __file__)
    dest.write(small_switch.getSwitch() + '\n')

if src:
    src.close()
dest.close()
