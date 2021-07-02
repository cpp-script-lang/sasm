#pragma once
/**
* Naming:
* <name>X = do <name> <operand 1> times
* <name>P = do <name> and pop operands
* <name>B = bitwise
* <name>L = logical
* R<name> = do <name>, but with reversed operands
* <name>I = do <name> with operands of type integer
* <name>C = do <name> with operands of type char
* <name>S = do <name> with operands of type string
* <name>B = do <name> with operands of type bool
* <name>F = do <name> with operands of type with floating point
* <name>S = do <name> with operands of type string
*/
enum class INSTR
{
    // Stack operations
    LOAD = 0x00 /* unary */, POP /* nullary */, POPX /* unary */,
    // Arithmetic operations
    INC, DEC, NEGI, NEGF, ADDI, ADDF, SUBI, SUBF, RSUBI, RSUBF, MULI, MULF, DIV, RDIV, MOD, POW, RPOW, SQUARE, CUBE, DOUBLEI, DOUBLEF, TRIPLEI, TRIPLEF, // nullary
    // Bitwise operations
    ANDB, ORB, XORB, NANDB, NORB, NOTB,
    // Logical operations
    ANDL, ORL, XORL, NANDL, NORL, NOTL, IMPL, CONGR,
    // Control flow
    CALL /* unary */, IF /* nullary */, ENDIF /* nullary */,
    // Array operations
    IDXI, IDXC, IDXB, IDXF, // unary
    // I/O
    PRINTI, PRINTC, PRINTS, PRINTB, PRINTF, PRINTPI, PRINTPC, PRINTPS, PRINTPB, PRINTPF, PRINTX, SCANI, SCANC, SCANS, SCANF, // all except PRINTX nullary, but PRINTX unary
    // Miscellaneous
    RET, NOP, DBG, CONC, CLR, // nullary
    // Relational (ordering) operations
    EQ, EQP, GT, GTP, RGT, RGTP, GTE, GTEP, RGTE, RGTEP, LT, LTP, RLT, RLTP, LTE, LTEP, RLTE, RLTEP, // nullary
    UNKNOWN_OPCODE, COMMENT, EMPTYLINE
};