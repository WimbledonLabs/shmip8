switch (op.digit0) {
case 0x0: switch (op.digit1) {
	case 0x0: switch (op.digit3) {
		case 0x0: std::cout << "CLR"; break;
		case 0xE: std::cout << "RET"; break;
		default: std::cout << "NOP"; break;
		} break;
	default: std::cout << "RCA " << "#" << std::hex << ((int) op.nNN) << "";	 break;
	} break;
case 0x1: std::cout << "JP " << "#" << std::hex << ((int) op.nNN) << ""; break;
case 0x2: std::cout << "CALL " << "#" << std::hex << ((int) op.nNN) << ""; break;
case 0x3: std::cout << "SE V" << std::hex << ((int) op.vX) << ", " << "#" << std::hex << ((int) op.nN) << ""; break;
case 0x4: std::cout << "SNE V" << std::hex << ((int) op.vX) << ", " << "#" << std::hex << ((int) op.nN) << ""; break;
case 0x5: std::cout << "SE V" << std::hex << ((int) op.vX) << ", V" << std::hex << ((int) op.vY) << ""; break;
case 0x6: std::cout << "MOV V" << std::hex << ((int) op.vX) << ", " << "#" << std::hex << ((int) op.nN) << ""; break;
case 0x7: std::cout << "ADD V" << std::hex << ((int) op.vX) << ", " << "#" << std::hex << ((int) op.nN) << ""; break;
case 0x8: switch (op.digit3) {
	case 0x0: std::cout << "MOV V" << std::hex << ((int) op.vX) << ", V" << std::hex << ((int) op.vY) << ""; break;
	case 0x1: std::cout << "OR V" << std::hex << ((int) op.vX) << ", V" << std::hex << ((int) op.vY) << ""; break;
	case 0x2: std::cout << "AND V" << std::hex << ((int) op.vX) << ", V" << std::hex << ((int) op.vY) << ""; break;
	case 0x3: std::cout << "XOR V" << std::hex << ((int) op.vX) << ", V" << std::hex << ((int) op.vY) << ""; break;
	case 0x4: std::cout << "ADD V" << std::hex << ((int) op.vX) << ", V" << std::hex << ((int) op.vY) << ""; break;
	case 0x5: std::cout << "SUB V" << std::hex << ((int) op.vX) << ", V" << std::hex << ((int) op.vY) << ""; break;
	case 0x6: std::cout << "RSH V" << std::hex << ((int) op.vX) << ""; break;
	case 0x7: std::cout << "RSUB V" << std::hex << ((int) op.vX) << ", V" << std::hex << ((int) op.vY) << ""; break;
	case 0xE: std::cout << "LSH " << std::hex << ((int) op.vX) << ""; break;
	default: std::cout << "NOP"; break;
	} break;
case 0x9: std::cout << "SNE V" << std::hex << ((int) op.vX) << ", V" << std::hex << ((int) op.vY) << ""; break;
case 0xA: std::cout << "MOV I, " << "#" << std::hex << ((int) op.nNN) << ""; break;
case 0xB: std::cout << "B V" << std::hex << ((int) op.vX) << ", " << "#" << std::hex << ((int) op.nNN) << ""; break;
case 0xC: std::cout << "RAND V" << std::hex << ((int) op.vX) << ", " << "#" << std::hex << ((int) op.nN) << ""; break;
case 0xD: std::cout << "DRW V" << std::hex << ((int) op.vX) << ", V" << std::hex << ((int) op.vY) << ", " << "#" << std::hex << ((int) op.n) << ""; break;
case 0xE: switch (op.digit2) {
	case 0x9: std::cout << "SKP V" << std::hex << ((int) op.vX) << ""; break;
	case 0xA: std::cout << "SKNP V" << std::hex << ((int) op.vX) << ""; break;
	default: std::cout << "NOP"; break;
	} break;
case 0xF: switch (op.digit2) {
	case 0x0: switch (op.digit3) {
		case 0x7: std::cout << "MOV V" << std::hex << ((int) op.vX) << ", DT"; break;
		case 0xA: std::cout << "WAIT V" << std::hex << ((int) op.vX) << ""; break;
		default: std::cout << "NOP"; break;
		} break;
	case 0x1: switch (op.digit3) {
		case 0x5: std::cout << "MOV DT, V" << std::hex << ((int) op.vX) << ""; break;
		case 0x8: std::cout << "MOV ST, V" << std::hex << ((int) op.vX) << ""; break;
		case 0xE: std::cout << "ADD I, V" << std::hex << ((int) op.vX) << ""; break;
		default: std::cout << "NOP"; break;
		} break;
	case 0x2: std::cout << "MOV F, V" << std::hex << ((int) op.vX) << ""; break;
	case 0x3: std::cout << "BCD V" << std::hex << ((int) op.vX) << ""; break;
	case 0x5: std::cout << "PUSH V" << std::hex << ((int) op.vX) << ""; break;
	case 0x6: std::cout << "POP V" << std::hex << ((int) op.vX) << ""; break;
	default: std::cout << "NOP"; break;
	} break;
default: std::cout << "NOP"; break;
}
