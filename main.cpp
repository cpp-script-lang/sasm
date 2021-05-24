/**
* CVM:
* |@Front-end@@@@@@@|
* ------------------|
* |@@@CILR@@@@@@@@@@|
* |   CVM           |
* |###OLR#(bez#OOP)#|
* ------------------|
* |#OLE#(OTHERLANG)#|
* |###Old-langs#####|
*/
/**
* @file main.cpp
* @proj SASM
* @brief Stack Assembly language
* @date 28.04.2021
*/
#include<stack>
#include<iostream>
#include<fstream>
#include<any>
#include<stack>
#include<type_traits>
#include<vector>
#include<cmath>

std::stack<std::any> stack{};
std::string line{};
std::string file_name{};
std::size_t line_num = 0;
std::vector<std::string> instruction{};
INSTR mnemonic{};
std::any op1{}, op2{}, op3{}, op4{};
enum class STATUS
{
    UNRECOGNISEDARG = -390, ARGUMENTSOVERFLOW = 3, ARGUMENTSUNDERFLOW = -3, DIVBYZERO = -1, SUCCESS = 0, ISNOTARRAY = 1, UNDECLAREDID = 2, NOFILE = -2, EMPTYSTACK = 390
};
/**
* Naming:
* <name>X = do <name> <operand 1> times
* <name>P = do <name> and pop operands
* <name>B = bitwise
* <name>L = logical
* R<name> = do <name>, but with reversed operands
* <name>I = do <name> with operands of type integer
* <name>C = do <name> with operands of type char or char[]
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
    PRINTI, PRINTC, PRINTB, PRINTF, PRINTPI, PRINTPC, PRINTPB, PRINTPF, PRINTX, SCANI, SCANC, SCANS, SCANF, // all except PRINTX nullary, but PRINTX unary
    // Miscellaneous
    RET, NOP, DBG, CONC, CLR, // nullary
    // Relational (ordering) operations
    EQ, EQP, GT, GTP, RGT, RGTP, GTE, GTEP, RGTE, RGTEP, LT, LTP, RLT, RLTP, LTE, LTEP, RLTE, RLTEP, // nullary
    UNKNOWN_OPCODE
};
STATUS WrongArity(const std::string& file_name, const std::string& line, const std::size_t& line_num, const std::string& instr, const unsigned short& corr_args, const unsigned short& actual_args)
{
    std::cerr << "In file " << file_name << ": line: " << line_num << ": error: '" << instr << "' statement accepts only " << corr_args << " operand" << (corr_args != 1 ? "s" : "") << ", but " << actual_args << " were given!" << std::endl;
    std::cerr << line << std::endl;
    return (corr_args < actual_args ? STATUS::ARGUMENTSOVERFLOW : STATUS::ARGUMENTSUNDERFLOW);
}
STATUS UndeclaredID(const std::string& file_name, const std::string& line, const std::size_t& line_num, const std::string& instr)
{
    std::cerr << "In file " << file_name << ": line: " << line_num << ": error: Undeclared identifier: '" << instr << "'!" << std::endl;
    std::cerr << line << std::endl;
    return STATUS::UNDECLAREDID;
}
STATUS EmptyStack(const std::string& file_name, const std::string& line, const std::size_t& line_num, const std::string& instr, const std::size_t& stack_size)
{
    std::cerr << "In file " << file_name << ": line: " << line_num << ": error: Stack is empty, but '" << instr << "' was used!" << std::endl;
    std::cerr << line << std::endl;
    return STATUS::EMPTYSTACK;
}
INSTR GetInstrFromString(const std::string& instr)
{
    if(instr == "LOAD" || instr == "load") return INSTR::LOAD;
    else if(instr == "POP" || instr == "pop") return INSTR::POP;
    else if(instr == "POPX" || instr == "popx") return INSTR::POPX;
    else if(instr == "INC" || instr == "inc") return INSTR::INC;
    else if(instr == "DEC" || instr == "dec") return INSTR::DEC;
    else if(instr == "NEGI" || instr == "negi") return INSTR::NEGI;
    else if(instr == "NEGF" || instr == "negf") return INSTR::NEGF;
    else if(instr == "ADDI" || instr == "addi") return INSTR::ADDI;
    else if(instr == "ADDF" || instr == "addf") return INSTR::ADDF;
    else if(instr == "SUBI" || instr == "subi") return INSTR::SUBI;
    else if(instr == "SUBF" || instr == "subf") return INSTR::SUBF;
    else if(instr == "RSUBI" || instr == "rsubi") return INSTR::RSUBI;
    else if(instr == "RSUBF" || instr == "rsubf") return INSTR::RSUBF;
    else if(instr == "MULI" || instr == "muli") return INSTR::MULI;
    else if(instr == "MULF" || instr == "mulf") return INSTR::MULF;
    else if(instr == "DIV" || instr == "div") return INSTR::DIV;
    else if(instr == "RDIV" || instr == "rdiv") return INSTR::RDIV;
    else if(instr == "MOD" || instr == "mod") return INSTR::MOD;
    else if(instr == "POW" || instr == "pow") return INSTR::POW;
    else if(instr == "RPOW" || instr == "rpow") return INSTR::RPOW;
    else if(instr == "SQUARE" || instr == "square") return INSTR::SQUARE;
    else if(instr == "CUBE" || instr == "cube") return INSTR::CUBE;
    else if(instr == "DOUBLEI" || instr == "doublei") return INSTR::DOUBLEI;
    else if(instr == "DOUBLEF" || instr == "doublef") return INSTR::DOUBLEF;
    else if(instr == "TRIPLEI" || instr == "triplei") return INSTR::TRIPLEI;
    else if(instr == "TRIPLEF" || instr == "triplef") return INSTR::TRIPLEF; 
    else if(instr == "ANDB" || instr == "andb") return INSTR::ANDB;
    else if(instr == "ORB" || instr == "orb") return INSTR::ORB;
    else if(instr == "XORB" || instr == "xorb") return INSTR::XORB;
    else if(instr == "NANDB" || instr == "nandb") return INSTR::NANDB;
    else if(instr == "NORB" || instr == "norb") return INSTR::NORB;
    else if(instr == "NOTB" || instr == "notb") return INSTR::NOTB;
    else if(instr == "ANDL" || instr == "andl") return INSTR::ANDL;
    else if(instr == "ORL" || instr == "orl") return INSTR::ORL;
    else if(instr == "XORL" || instr == "xorl") return INSTR::XORL;
    else if(instr == "NANDL" || instr == "nandl") return INSTR::NANDL;
    else if(instr == "NORL" || instr == "norl") return INSTR::NORL;
    else if(instr == "NOTL" || instr == "notl") return INSTR::NOTL;
    else if(instr == "IMPL" || instr == "impl") return INSTR::IMPL;
    else if(instr == "CONGR" || instr == "congr") return INSTR::CONGR;
    else if(instr == "CALL" || instr == "call") return INSTR::CALL;
    else if(instr == "IF" || instr == "if") return INSTR::IF;
    else if(instr == "ENDIF" || instr == "endif") return INSTR::ENDIF;
    else if(instr == "IDXI" || instr == "idxi") return INSTR::IDXI;
    else if(instr == "IDXC" || instr == "idxc") return INSTR::IDXC;
    else if(instr == "IDXB" || instr == "idxb") return INSTR::IDXB;
    else if(instr == "IDXF" || instr == "idxf") return INSTR::IDXF;
    else if(instr == "PRINTI" || instr == "printi") return INSTR::PRINTI;
    else if(instr == "PRINTC" || instr == "printc") return INSTR::PRINTC;
    else if(instr == "PRINTB" || instr == "printb") return INSTR::PRINTB;
    else if(instr == "PRINTF" || instr == "printf") return INSTR::PRINTF;
    else if(instr == "PRINTPI" || instr == "printpi") return INSTR::PRINTPI;
    else if(instr == "PRINTPC" || instr == "printpc") return INSTR::PRINTPC;
    else if(instr == "PRINTPB" || instr == "printpb") return INSTR::PRINTPB;
    else if(instr == "PRINTPF" || instr == "printpf") return INSTR::PRINTPF;
    else if(instr == "SCANI" || instr == "scani") return INSTR::SCANI;
    else if(instr == "SCANC" || instr == "scanc") return INSTR::SCANC;
    else if(instr == "SCANS" || instr == "scans") return INSTR::SCANS;
    else if(instr == "SCANF" || instr == "scanf") return INSTR::SCANF;
    else if(instr == "RET" || instr == "ret") return INSTR::RET;
    else if(instr == "NOP" || instr == "nop") return INSTR::NOP;
    else if(instr == "DBG" || instr == "dbg") return INSTR::DBG;
    else if(instr == "CONC" || instr == "conc") return INSTR::CONC;
    else if(instr == "CLR" || instr == "clr") return INSTR::CLR;
    else if(instr == "EQ" || instr == "eq") return INSTR::EQ;
    else if(instr == "EQP" || instr == "eqp") return INSTR::EQP;
    else if(instr == "GT" || instr == "gt") return INSTR::GT;
    else if(instr == "GTP" || instr == "gtp") return INSTR::GTP;
    else if(instr == "RGT" || instr == "rgt") return INSTR::RGT;
    else if(instr == "RGTP" || instr == "rgtp") return INSTR::RGTP;
    else if(instr == "GTE" || instr == "gte") return INSTR::GTE;
    else if(instr == "GTEP" || instr == "gtep") return INSTR::GTEP;
    else if(instr == "RGTE" || instr == "rgte") return INSTR::RGTE;
    else if(instr == "RGTEP" || instr == "rgtep") return INSTR::RGTEP;
    else if(instr == "LT" || instr == "lt") return INSTR::LT;
    else if(instr == "LTP" || instr == "ltp") return INSTR::LTP;
    else if(instr == "RLT" || instr == "rlt") return INSTR::RLT;
    else if(instr == "RLTP" || instr == "rltp") return INSTR::RLTP;
    else if(instr == "LTE" || instr == "lte") return INSTR::LTE;
    else if(instr == "LTEP" || instr == "ltep") return INSTR::LTEP;
    else if(instr == "RLTE" || instr == "rlte") return INSTR::RLTE;
    else if(instr == "RLTEP" || instr == "rltep") return INSTR::RLTEP;
    else return INSTR::UNKNOWN_OPCODE;
}
// is_array

template<typename T, typename = void>
struct is_array : std::false_type {};

template<typename T>
struct is_array<T, std::void_t<decltype(std::declval<T>()[1])>> : std::true_type {};

/**
* @function Split
* @param[in] str
* @brief Split str into space-separated words
*/

std::vector<std::string> Split(const std::string& str)
{
    std::vector<std::string> words{};
    for(auto i = 0; i < str.size(); i++)
    {
        if(str[i] == ',' || str[i] == ' ') continue;
        words[i].push_back(str[i]);
    }
    return words;
}
STATUS interpret(const std::ifstream& file, const std::string& file_name, const std::vector<std::string>& argv = {})
{
    while(!file.eof())
    {
        std::getline(const_cast<std::ifstream&>(file), line);
        line_num++;
        instruction = Split(line);
        mnemonic = GetInstrFromString(instruction[0]);
        switch(mnemonic)
        {
        case INSTR::LOAD:
            if(instruction.size() != 2) return WrongArity(file_name, line, line_num, "load", 1, instruction.size() - 1);
            stack.push(instruction[1]);
            break;
        case INSTR::POP:
            if(instruction.size() > 1) return WrongArity(file_name, line, line_num, "pop", 0, instruction.size() - 1);
            if(stack.empty()) return EmptyStack(file_name, line, line_num, "pop", stack.size());
            stack.pop();
            break;
        case INSTR::POPX:
            if(instruction.size() != 2) return WrongArity(file_name, line, line_num, "popx", 1, instruction.size() - 1);
            op1 = instruction[1];
            for(auto i = 0; i < std::any_cast<decltype(i)>(op1); i++)
            {
                if(stack.empty()) return EmptyStack(file_name, line, line_num, "popx", stack.size());
                stack.pop();
            }
            break;
        case INSTR::INC:
            if(instruction.size() > 1) return WrongArity(file_name, line, line_num, "inc", 0, instruction.size() - 1);
            op1 = stack.top(); stack.pop(); stack.push(++std::any_cast<long double&>(op1));
            break;
        case INSTR::DEC:
            if(instruction.size() > 1) return WrongArity(file_name, line, line_num, "dec", 0, instruction.size() - 1);
            op1 = stack.top(); stack.pop(); stack.push(--std::any_cast<long double&>(op1));
            break;
        case INSTR::NEGI:
            if(instruction.size() > 1) return WrongArity(file_name, line, line_num, "negi", 0, instruction.size() - 1);
            op1 = stack.top(); stack.pop(); stack.push(-std::any_cast<long long>(op1));
            break;
        case INSTR::NEGF:
            if(instruction.size() > 1) return WrongArity(file_name, line, line_num, "negf", 0, instruction.size() - 1);
            op1 = stack.top(); stack.pop(); stack.push(-std::any_cast<long double>(op1));
            break;
        case INSTR::ADDI:
            if(instruction.size() > 1) return WrongArity(file_name, line, line_num, "addi", 0, instruction.size() - 1);
            op1 = stack.top(); stack.pop(); op2 = stack.top(); stack.pop(); stack.push(std::any_cast<long long>(op1) + std::any_cast<long long>(op2));
            break;
        case INSTR::ADDF:
            if(instruction.size() > 1) return WrongArity(file_name, line, line_num, "addf", 0, instruction.size() - 1);
            op1 = stack.top(); stack.pop(); op2 = stack.top(); stack.pop(); stack.push(std::any_cast<long double>(op1) + std::any_cast<long double>(op2));
            break;
        case INSTR::SUBI:
            if(instruction.size() > 1) return WrongArity(file_name, line, line_num, "subi", 0, instruction.size() - 1);
            op1 = stack.top(); stack.pop(); op2 = stack.top(); stack.pop(); stack.push(std::any_cast<long long>(op1) - std::any_cast<long long>(op2));
            break;
        case INSTR::SUBF:
            if(instruction.size() > 1) return WrongArity(file_name, line, line_num, "subf", 0, instruction.size() - 1);
            op1 = stack.top(); stack.pop(); op2 = stack.top(); stack.pop(); stack.push(std::any_cast<long double>(op1) - std::any_cast<long double>(op2));
            break;
        case INSTR::RSUBI:
            if(instruction.size() > 1) return WrongArity(file_name, line, line_num, "rsubi", 0, instruction.size() - 1);
            op1 = stack.top(); stack.pop(); op2 = stack.top(); stack.pop(); stack.push(std::any_cast<long long>(op2) - std::any_cast<long long>(op1));
            break;
        case INSTR::RSUBF:
            if(instruction.size() > 1) return WrongArity(file_name, line, line_num, "rsubf", 0, instruction.size() - 1);
            op1 = stack.top(); stack.pop(); op2 = stack.top(); stack.pop(); stack.push(std::any_cast<long double>(op2) - std::any_cast<long double>(op1));
            break;
        case INSTR::MULI:
            if(instruction.size() > 1) return WrongArity(file_name, line, line_num, "muli", 0, instruction.size() - 1);
            op1 = stack.top(); stack.pop(); op2 = stack.top(); stack.pop(); stack.push(std::any_cast<long long>(op1) * std::any_cast<long long>(op2));
            break;
        case INSTR::MULF:
            if(instruction.size() > 1) return WrongArity(file_name, line, line_num, "mulf", 0, instruction.size() - 1);
            op1 = stack.top(); stack.pop(); op2 = stack.top(); stack.pop(); stack.push(std::any_cast<long double>(op1) * std::any_cast<long double>(op2));
            break;
        case INSTR::DOUBLEI:
            if(instruction.size() > 1) return WrongArity(file_name, line, line_num, "doublei", 0, instruction.size() - 1);
            op1 = stack.top(); stack.pop(); stack.push(2 * std::any_cast<long long>(op1));
            break;
        case INSTR::DOUBLEF:
            if(instruction.size() > 1) return WrongArity(file_name, line, line_num, "doublef", 0, instruction.size() - 1);
            op1 = stack.top(); stack.pop(); stack.push(2. * std::any_cast<long double>(op1));
            break;
        case INSTR::TRIPLEI:
            if(instruction.size() > 1) return WrongArity(file_name, line, line_num, "triplei", 0, instruction.size() - 1);
            op1 = stack.top(); stack.pop(); stack.push(3 * std::any_cast<long long>(op1));
            break;
        case INSTR::TRIPLEF:
            if(instruction.size() > 1) return WrongArity(file_name, line, line_num, "triplef", 0, instruction.size() - 1);
            op1 = stack.top(); stack.pop(); stack.push(3. * std::any_cast<long double>(op1));
            break;
        case INSTR::DIV:
            if(instruction.size() > 1) return WrongArity(file_name, line, line_num, "div", 0, instruction.size() - 1);
            op1 = stack.top(); stack.pop(); op2 = stack.top(); stack.pop();
            if(std::any_cast<int>(op2) == 0)
            {
                stack.push(op2); stack.push(op1);
                return DivisionByZero(file_name, line, line_num);
            }
            stack.push(std::any_cast<long double>(op1) / std::any_cast<long double>(op2));
            break;
       case INSTR::RDIV:
            if(instruction.size() > 1) return WrongArity(file_name, line, line_num, "rdiv", 0, instruction.size() - 1);
            if(std::any_cast<int>(stack.top()) == 0) return DivisionByZero(file_name, line, line_num);
            op1 = stack.top(); stack.pop(); op2 = stack.top(); stack.pop(); stack.push(std::any_cast<long double>(op2) / std::any_cast<long double>(op1));
            break;
       case INSTR::POW:
            if(instruction.size() > 1) return WrongArity(file_name, line, line_num, "pow", 0, instruction.size() - 1);
            op1 = stack.top(); stack.pop(); op2 = stack.top(); stack.pop(); stack.push(std::pow(std::any_cast<long double>(op1), std::any_cast<long double>(op2)));
            break;
       case INSTR::RPOW:
            if(instruction.size() > 1) return WrongArity(file_name, line, line_num, "rpow", 0, instruction.size() - 1);
            op1 = stack.top(); stack.pop(); op2 = stack.top(); stack.pop(); stack.push(std::pow(std::any_cast<long double>(op2), std::any_cast<long double>(op1)));
            break;
       case INSTR::SQUARE:
            if(instruction.size() > 1) return WrongArity(file_name, line, line_num, "square", 0, instruction.size() - 1);
            op1 = stack.top(); stack.pop(); stack.push(std::pow(std::any_cast<long double>(op1), 2));
            break;
       case INSTR::CUBE:
            if(instruction.size() > 1) return WrongArity(file_name, line, line_num, "cube", 0, instruction.size() - 1);
            op1 = stack.top(); stack.pop(); stack.push(std::pow(std::any_cast<long double>(op1), 3));
            break;
       case INSTR::ANDB:
            if(instruction.size() > 1) return WrongArity(file_name, line, line_num, "andb", 0, instruction.size() - 1);
            op1 = stack.top(); stack.pop(); op2 = stack.top(); stack.pop(); stack.push(std::any_cast<unsigned long long>(op1) & std::any_cast<unsigned long long>(op2));
            break;
       case INSTR::ORB:
            if(instruction.size() > 1) return WrongArity(file_name, line, line_num, "orb", 0, instruction.size() - 1);
            op1 = stack.top(); stack.pop(); op2 = stack.top(); stack.pop(); stack.push(std::any_cast<unsigned long long>(op1) | std::any_cast<unsigned long long>(op2));
            break;
       // TODO: Bitwise operations and other operations
       case INSTR::PRINTI:
            if(instruction.size() > 1) return WrongArity(file_name, line, line_num, "printi", 0, instruction.size() - 1);
            std::cout << std::any_cast<int>(stack.top());
            break;
       case INSTR::PRINTPI:
            if(instruction.size() > 1) return WrongArity(file_name, line, line_num, "printpi", 0, instruction.size() - 1);
            std::cout << std::any_cast<int>(stack.top()); stack.pop();
            break;
       case INSTR::SCANI:
            if(instruction.size() > 1) return WrongArity(file_name, line, line_num, "scani", 0, instruction.size() - 1);
            int x{}; std::cin >> x; stack.top() = x;
            break;
       case INSTR::IDXI:
            if(instruction.size() > 2) return WrongArity(file_name, line, line_num, "idxi", 1, instruction.size() - 1);
            if(!is_array<decltype(stack.top())>::value) return OperandIsNotArray(file_name, line, line_num, stack.top());
            op1 = instruction[1]; stack.push(std::any_cast<std::vector<std::any>>(stack.top())[std::any_cast<std::size_t>(op1)]);
       default:
            return UndeclaredID(file_name, line, line_num, instruction[0]);
       }
    }
}
STATUS help()
{
    std::cout << R"(Usage: sasm[.exe | .out | ] [-h | --help | -v | --version | <FILENAME>]
OPTIONS
    -h, --help      displays this help and exits
    -v, --version  displays version and program info and exits
    FILENAME        file to interpret
)";
    return STATUS::SUCCESS;
}
STATUS version()
{
    std::cout << R"(SASM (Stack Assembly) language interpreter
Version: 0.1.0-alpha
Author: Antoni Kiedos
Issue tracker: https://github.com/cpp-script-lang/sasm/issues
Contributing: https://github.com/cpp-script-lang/sasm/pulls
License: GNU GPLv3 or higher
--------------------------------------------------------------
Copyright (C) Antoni Kiedos 2021 - )" << tm().tm_year << ", Poland" << std::endl;
    return STATUS::SUCCESS;
}
STATUS ExecuteCommand(const std::string& cmd)
{
    if(cmd == "--help" || cmd == "-h") return help();
    else if(cmd == "--version" || cmd == "-v") return version();
    else return STATUS::UNRECOGNISEDARG;
}
bool IsValidFile(const std::string& file_name)
{
    return file_name.substr(file_name.size() - 5, 5) == ".sasm";
}
int main(int argc, char** argv)
{
    if(argc == 2)
    {
        file_name = argv[1];
        std::ifstream infile(file_name);
        if(IsValidFile(file_name)) return static_cast<int>(interpret(infile, file_name));
        else return static_cast<int>(ExecuteCommand(file_name));
    }
    else if(argc > 2)
    {
        std::vector<std::string> arr_argv{};
        for(auto i = 2; i < argc; i++) arr_argv.push_back(argv[i]);
        std::ifstream infile(argv[1]);
        return static_cast<int>(interpret(infile, std::string(argv[1]), arr_argv));
    }
    else return static_cast<int>(STATUS::NOFILE);
}
