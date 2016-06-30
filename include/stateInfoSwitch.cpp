switch (op.digit0) {
case 0x0: switch (op.digit1) {
	case 0x0: switch (op.digit3) {
		case 0x0:  break;
		case 0xE: std::cout << "pc: " << std::hex << "0x" << ((int) core->pc); break;
		default: std::cout << "NOP"; break;
		} break;
	default: 	 break;
	} break;
case 0x1: std::cout << "pc: " << std::hex << "0x" << ((int) core->pc); break;
case 0x2: std::cout << "pc: " << std::hex << "0x" << ((int) core->pc); break;
case 0x3: std::cout << "vX: " << std::hex << "0x"  << ((int) core->reg[op.vX]) << "  " << "pc: " << std::hex << "0x" << ((int) core->pc); break;
case 0x4: std::cout << "vX: " << std::hex << "0x"  << ((int) core->reg[op.vX]) << "  " << "pc: " << std::hex << "0x" << ((int) core->pc); break;
case 0x5: std::cout << "vX: " << std::hex << "0x"  << ((int) core->reg[op.vX]) << "  " << "vY: " << std::hex << "0x" << ((int) core->reg[op.vY]) << "  " << "pc: " << std::hex << "0x" << ((int) core->pc); break;
case 0x6: std::cout << "vX: " << std::hex << "0x"  << ((int) core->reg[op.vX]); break;
case 0x7: std::cout << "vX: " << std::hex << "0x"  << ((int) core->reg[op.vX]); break;
case 0x8: switch (op.digit3) {
	case 0x0: std::cout << "vX: " << std::hex << "0x"  << ((int) core->reg[op.vX]) << "  " << "vY: " << std::hex << "0x" << ((int) core->reg[op.vY]); break;
	case 0x1: std::cout << "vX: " << std::hex << "0x"  << ((int) core->reg[op.vX]) << "  " << "vY: " << std::hex << "0x" << ((int) core->reg[op.vY]); break;
	case 0x2: std::cout << "vX: " << std::hex << "0x"  << ((int) core->reg[op.vX]) << "  " << "vY: " << std::hex << "0x" << ((int) core->reg[op.vY]); break;
	case 0x3: std::cout << "vX: " << std::hex << "0x"  << ((int) core->reg[op.vX]) << "  " << "vY: " << std::hex << "0x" << ((int) core->reg[op.vY]); break;
	case 0x4: std::cout << "vX: " << std::hex << "0x"  << ((int) core->reg[op.vX]) << "  " << "vY: " << std::hex << "0x" << ((int) core->reg[op.vY]) << "  " << "vF: " << std::hex << "0x" << ((int) core->reg[0xF]); break;
	case 0x5: std::cout << "vX: " << std::hex << "0x"  << ((int) core->reg[op.vX]) << "  " << "vY: " << std::hex << "0x" << ((int) core->reg[op.vY]) << "  " << "vF: " << std::hex << "0x" << ((int) core->reg[0xF]); break;
	case 0x6: std::cout << "vX: " << std::hex << "0x"  << ((int) core->reg[op.vX]) << "  " << "vY: " << std::hex << "0x" << ((int) core->reg[op.vY]) << "  " << "vF: " << std::hex << "0x" << ((int) core->reg[0xF]); break;
	case 0x7: std::cout << "vX: " << std::hex << "0x"  << ((int) core->reg[op.vX]) << "  " << "vY: " << std::hex << "0x" << ((int) core->reg[op.vY]) << "  " << "vF: " << std::hex << "0x" << ((int) core->reg[0xF]); break;
	case 0xE: std::cout << "vX: " << std::hex << "0x"  << ((int) core->reg[op.vX]) << "  " << "vY: " << std::hex << "0x" << ((int) core->reg[op.vY]) << "  " << "vF: " << std::hex << "0x" << ((int) core->reg[0xF]); break;
	default: std::cout << "NOP"; break;
	} break;
case 0x9: std::cout << "vX: " << std::hex << "0x"  << ((int) core->reg[op.vX]) << "  " << "vY: " << std::hex << "0x" << ((int) core->reg[op.vY]) << "  " << "pc: " << std::hex << "0x" << ((int) core->pc); break;
case 0xA: std::cout << "I: " << std::hex << "0x" << ((int) core->I); break;
case 0xB: std::cout << "vX: " << std::hex << "0x"  << ((int) core->reg[op.vX]) << "  " << "pc: " << std::hex << "0x" << ((int) core->pc); break;
case 0xC: std::cout << "vX: " << std::hex << "0x"  << ((int) core->reg[op.vX]); break;
case 0xD: std::cout << "vX: " << std::hex << "0x"  << ((int) core->reg[op.vX]) << "  " << "vY: " << std::hex << "0x" << ((int) core->reg[op.vY]) << "  " << "I: " << std::hex << "0x" << ((int) core->I); break;
case 0xE: switch (op.digit2) {
	case 0x9: std::cout << "vX: " << std::hex << "0x"  << ((int) core->reg[op.vX]) << "  " << "keys: " << core->keyboard.keys; break;
	case 0xA: std::cout << "vX: " << std::hex << "0x"  << ((int) core->reg[op.vX]) << "  " << "keys: " << core->keyboard.keys; break;
	default: std::cout << "NOP"; break;
	} break;
case 0xF: switch (op.digit2) {
	case 0x0: switch (op.digit3) {
		case 0x7: std::cout << "vX: " << std::hex << "0x"  << ((int) core->reg[op.vX]) << "  " << "dT: " << std::hex << "0x" << ((int) core->dT); break;
		case 0xA: std::cout << "vX: " << std::hex << "0x"  << ((int) core->reg[op.vX]); break;
		default: std::cout << "NOP"; break;
		} break;
	case 0x1: switch (op.digit3) {
		case 0x5: std::cout << "vX: " << std::hex << "0x"  << ((int) core->reg[op.vX]) << "  " << "dT: " << std::hex << "0x" << ((int) core->dT); break;
		case 0x8: std::cout << "vX: " << std::hex << "0x"  << ((int) core->reg[op.vX]) << "  " << "sT: " << std::hex << "0x" << ((int) core->sT); break;
		case 0xE: std::cout << "vX: " << std::hex << "0x"  << ((int) core->reg[op.vX]) << "  " << "I: " << std::hex << "0x" << ((int) core->I); break;
		default: std::cout << "NOP"; break;
		} break;
	case 0x2: std::cout << "vX: " << std::hex << "0x"  << ((int) core->reg[op.vX]) << "  " << "I: " << std::hex << "0x" << ((int) core->I); break;
	case 0x3: std::cout << "vX: " << std::hex << "0x"  << ((int) core->reg[op.vX]) << "  " << "BCD: " << BCD(core, op); break;
	case 0x5: std::cout << "vX: " << std::hex << "0x"  << ((int) core->reg[op.vX]) << "  " << "regUpToVX: " << regUpToVX(core, op) << "  " << "ramUpToVX: " << ramUpToVX(core, op); break;
	case 0x6: std::cout << "vX: " << std::hex << "0x"  << ((int) core->reg[op.vX]) << "  " << "regUpToVX: " << regUpToVX(core, op) << "  " << "ramUpToVX: " << ramUpToVX(core, op); break;
	default: std::cout << "NOP"; break;
	} break;
default: std::cout << "NOP"; break;
}
