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

enum class STATUS
{
    ARGUMENTSOVERFLOW = 3, ARGUMENTSUNDERFLOW = -3, DIVBYZERO = -1, ISNOTARRAY = 1, UNDECLAREDID = 2, NOFILE = -2
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
    INC, DEC, NEGI, NEGF, ADDI, ADDF, SUBI, SUBF, RSUBI, RSUBF, MULI, MULF, DIV, RDIV, MOD, POW, RPOW, SQUARE, CUBE, DOUBLEI, DOUBLEF, TRIPLEI, TRIPLEF // nullary
    // Bitwise operations
    ANDB, ORB, XORB, NANDB, NORB, NOTB,
    // Logical operations
    ANDL, ORL, XORL, NANDL, NORL, NOTL, IMPL, CONGR,
    // Control flow
    CALL /* unary */, IF /* nullary */, ENDIF /* nullary */,
    // Array operations
    IDXI, IDXC, IDXB, IDXF, // unary
    // I/O
    PRINTI, PRINTC, PRINTB, PRINTF, PRINTPI, PRINTPC, PRINTPB, PRINTPF, PRINTX, SCANI, SCANC, SCANS, SCANF // all except PRINTX nullary, but PRINTX unary
    // Miscellaneous
    RET, NOP, DBG, CONC, CLR, // nullary
    // Relational (ordering) operations
    EQ, EQP, GT, GTP, RGT, RGTP, GTE, GTEP, RGTE, RGTEP, LT, LTP, RLT, RLTP, LTE, LTEP, RLTE, RLTEP // nullary
};

INSTR GetInstrFromString(const std::string& instr)
{
switch(instr)
{
    case "LOAD": case "load": return INSTR::LOAD;
    case "POP": case "pop": return INSTR::POP;
    case "POPX": case "popx": return INSTR::POPX;
    case "INC": case "inc": return INSTR::INC;
    case "DEC": case "dec": return INSTR::DEC;
    case "NEGI": case "negi": return INSTR::NEGI;
    case "NEGF": case "negf": return INSTR::NEGF;
    case "ADDI": case "addi": return INSTR::ADDI;
    case "ADDF": case "addf": return INSTR::ADDF;
    case "SUBI": case "subi": return INSTR::SUBI;
    case "SUBF": case "subf": return INSTR::SUBF;
    case "RSUBI": case "rsubi": return INSTR::RSUBI;
    case "RSUBF": case "rsubf": return INSTR::RSUBF;
    case "MULI": case "muli": return INSTR::MULI;
    case "MULF": case "mulf": return INSTR::MULF;
    case "DIV": case "div": return INSTR::DIV;
    case "RDIV": case "rdiv": return INSTR::RDIV;
    case "MOD": case "mod": return INSTR::MOD;
    case "POW": case "pow": return INSTR::POW;
    case "RPOW": case "rpow": return INSTR::RPOW;
    case "SQUARE": case "square": return INSTR::SQUARE;
    case "CUBE": case "cube": return INSTR::CUBE;
    case "DOUBLEI": case "doublei": return INSTR::DOUBLEI;
    case "DOUBLEF": case "doublef": return INSTR::DOUBLEF;
    case "TRIPLEI": case "triplei": return INSTR::TRIPLEI;
    case "TRIPLEF": case "triplef": return INSTR::TRIPLEF; 
    case "ANDB": case "andb": return INSTR::ANDB;
    case "ORB": case "orb": return INSTR::ORB;
    case "XORB": case "xorb": return INSTR::XORB;
    case "NANDB": case "nandb": return INSTR::NANDB;
    case "NORB": case "norb": return INSTR::NORB;
    case "NOTB": case "notb": return INSTR::NOTB;
    case "ANDL": case "andl": return INSTR::ANDL;
    case "ORL": case "orl": return INSTR::ORL;
    case "XORL": case "xorl": return INSTR::XORL;
    case "NANDL": case "nandl": return INSTR::NANDL;
    case "NORL": case "norl": return INSTR::NORL;
    case "NOTL": case "notl": return INSTR::NOTL;
    case "IMPL": case "impl": return INSTR::IMPL;
    case "CONGR": case "congr": return INSTR::CONGR;
    case "CALL": case "call": return INSTR::CALL;
    case "IF": case "if": return INSTR::IF;
    case "ENDIF": case "endif": return INSTR::ENDIF;
    case "IDXI": case "idxi": return INSTR::IDXI;
    case "IDXC": case "idxc": return INSTR::IDXC;
    case "IDXB": case "idxb": return INSTR::IDXB;
    case "IDXF": case "idxf": return INSTR::IDXF;
    case "PRINTI": case "printi": return INSTR::PRINTI;
    case "PRINTC": case "printc": return INSTR::PRINTC;
    case "PRINTB": case "printb": return INSTR::PRINTB;
    case "PRINTF": case "printf": return INSTR::PRINTF;
    case "PRINTPI": case "printpi": return INSTR::PRINTPI;
    case "PRINTPC": case "printpc": return INSTR::PRINTPC;
    case "PRINTPB": case "printpb": return INSTR::PRINTPB;
    case "PRINTPF": case "printpf": return INSTR::PRINTPF;
    case "SCANI": case "scani": return INSTR::SCANI;
    case "SCANC": case "scanc": return INSTR::SCANC;
    case "SCANS": case "scans": return INSTR::SCANS;
    case "SCANF": case "scanf": return INSTR::SCANF;
    case "RET": case "ret": return INSTR::RET;
    case "NOP": case "nop": return INSTR::NOP;
    case "DBG": case "dbg": return INSTR::DBG;
    case "CONC": case "conc": return INSTR::CONC;
    case "CLR": case "clr": return INSTR::CLR;
    case "EQ": case "eq": return INSTR::EQ;
    case "EQP": case "eqp": return INSTR::EQP;
    case "GT": case "gt": return INSTR::GT;
    case "GTP": case "gtp": return INSTR::GTP;
    case "RGT": case "rgt": return INSTR::RGT;
    case "RGTP": case "rgtp": return INSTR::RGTP;
    case "GTE": case "gte": return INSTR::GTE;
    case "GTEP": case "gtep": return INSTR::GTEP;
    case "RGTE": case "rgte": return INSTR::RGTE;
    case "RGTEP": case "rgtep": return INSTR::RGTEP;
    case "LT": case "lt": return INSTR::LT;
    case "LTP": case "ltp": return INSTR::LTP;
    case "RLT": case "rlt": return INSTR::RLT;
    case "RLTP": case "rltp": return INSTR::RLTP;
    case "LTE": case "lte": return INSTR::LTE;
    case "LTEP": case "ltep": return INTSR::LTEP;
    case "RLTE": case "rlte": return INSTR::RLTE;
    case "RLTEP": case "rltep": return INSTR::RLTEP;
    default:
         return UndeclaredID(instr);
    }
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
        if(str[i] == ' ') continue;
        words[i].push_back(str[i]);
    }
    return words;
}
STATUS WrongArity(const std::string& file_name, const size_t& line_num, const std::string& instr, const unsigned short& corr_args, const unsigned short& actual_args)
{
    std::cerr << "In file " << file_name << ": line: " << line_num << ": error: '" << instr << "' statement accepts only " << corr_args << " operand" << (corr_args !=/ 1 ? "s" : "") << ", but " << actual_args << " were given!\n";
    return (corr_args < actual_args ? STATUS::ARGUMENTSOVERFLOW : STATUS::ARGUMENTSUNDERFLOW);
}
STATUS interpret(const std::ifstream& file, const std::string& file_name, const std::vector<std::string>& argv = {})
{
    std::stack<std::any> stack;
    std::string line{};
    size_t line_num = 0;
    std::vector<std::string> instruction{};
    INSTR mnemonic{};
    std::any op1, op2, op3, op4;
    while(std::getline(file, line) != file.eof())
    {
        line_num++;
        instruction = Split(line);
        mnemonic = GetInstrFromString(instruction[0]);
        switch(mnemonic)
        {
        case INSTR::LOAD:
            if(instruction.size() != 2) return WrongArity(file_name, line_num, "load", 1, instruction.size() - 1);
            stack.push(instruction[1]);
            break;
        case INSTR::POP:
            if(instruction.size() > 1) return WrongArity(file_name, line_num, "pop", 0, instruction.size() - 1);
            stack.pop();
            break;
        case INSTR::POPX:
            if(instruction.size() != 2) return WrongArity(file_name, line_num, "popx", 1, instruction.size() - 1);
            op1 = instruction[1];
            for(auto i = 0; i < op1; i++)
            {
                if(stack.empty()) return EmptyStack(file_name, line_num, "popx", stack.size());
                stack.pop();
            }
            break;
        case INSTR::INC:
            if(instruction.size() > 1) return WrongArity(file_name, line_num, "inc", 0, instruction.size() - 1);
            op1 = stack.top(); stack.pop(); stack.push(++std::any_cast<long double>(op1));
            break;
        case INSTR::DEC:
            if(instruction.size() > 1) return WrongArity(file_name, line_num, "dec", 0, instruction.size() - 1);
            op1 = stack.top(); stack.pop(); stack.push(--std::any_cast<long double>(op1)]);
            break;
        case INSTR::NEGI:
            if(instruction.size() > 1) return WrongArity(file_name, line_num, "negi", 0, instruction.size() - 1);
            op1 = stack.top(); stack.pop(); stack.push(-std::any_cast<long long>(op1));
            break;
        case INSTR::NEGF:
            if(instruction.size() > 1) return WrongArity(file_name, line_num, "negf", 0, instruction.size() - 1);
            op1 = stack.top(); stack.pop(); stack.push(-std::any_cast<long double>(op1));
            break;
        case INSTR::ADDI:
            if(instruction.size() > 1) return WrongArity(file_name, line_num, "addi", 0, instruction.size() - 1);
            op1 = stack.top(); stack.pop(); op2 = stack.top(); stack.pop(); stack.push(std::any_cast<long long>(op1) + std::any_cast<long long>(op2));
            break;
        case INSTR::ADDF:
            if(instruction.size() > 1) return WrongArity(file_name, line_num, "addf", 0, instruction.size() - 1);
            op1 = stack.top(); stack.pop(); op2 = stack.top(); stack.pop(); stack.push(std::any_cast<long double>(op1) + std::any_cast<long double>(op2));
            break;
        case INSTR::SUBI:
            if(instruction.size() > 1) return WrongArity(file_name, line_num, "subi", 0, instruction.size() - 1);
            op1 = stack.top(); stack.pop(); op2 = stack.top(); stack.pop(); stack.push(std::any_cast<long long>(op1) - std::any_cast<long long>(op2));
            break;
        case INSTR::SUBF:
            if(instruction.size() > 1) return WrongArity(file_name, line_num, "subf", 0, instruction.size() - 1);
            op1 = stack.top(); stack.pop(); op2 = stack.top(); stack.pop(); stack.push(std::any_cast<long double>(op1) - std::any_cast<long double>(op2));
            break;
        case INSTR::RSUBI:
            if(instruction.size() > 1) return WrongArity(file_name, line_num, "rsubi", 0, instruction.size() - 1);
            op1 = stack.top(); stack.pop(); op2 = stack.top(); stack.pop(); stack.push(std::any_cast<long long>(op2) - std::any_cast<long long>(op1));
            break;
        case INSTR::RSUBF:
            if(instruction.size() > 1) return WrongArity(file_name, line_num, "rsubf", 0, instruction.size() - 1);
            op1 = stack.top(); stack.pop(); op2 = stack.top(); stack.pop(); stack.push(std::any_cast<long double>(op2) - std::any_cast<long double>(op1));
            break;
        case INSTR::MULI:
            if(instruction.size() > 1) return WrongArity(file_name, line_num, "muli", 0, instruction.size() - 1);
            op1 = stack.top(); stack.pop(); op2 = stack.top(); stack.pop(); stack.push(std::any_cast<long long>(op1) * std::any_cast<long long>(op2));
            break;
        case INSTR::MULF:
            if(instruction.size() > 1) return WrongArity(file_name, line_num, "mulf", 0, instruction.size() - 1);
            op1 = stack.top(); stack.pop(); op2 = stack.top(); stack.pop(); stack.push(std::any_cast<long double>(op1) * std::any_cast<long double>(op2));
            break;
        case INSTR::DOUBLEI:
            if(instruction.size() > 1) return WrongArity(file_name, line_num, "doublei", 0, instruction.size() - 1);
            op1 = stack.top(); stack.pop(); stack.push(2 * std::any_cast<long long>(op1));
            break;
        case INSTR::DOUBLEF:
            if(instruction.size() > 1) return WrongArity(file_name, line_num, "doublef", 0, instruction.size() - 1);
            op1 = stack.top(); stack.pop(); stack.push(2. * std::any_cast<long double>(op1));
            break;
        case INSTR::TRIPLEI:
            if(instruction.size() > 1) return WrongArity(file_name, line_num, "triplei", 0, instruction.size() - 1);
            op1 = stack.top(); stack.pop(); stack.push(3 * std::any_cast<long long>(op1));
            break;
        case INSTR::TRIPLEF:
            if(instruction.size() > 1) return WrongArity(file_name, line_num, "triplef", 0, instruction.size() - 1);
            op1 = stack.top(); stack.pop(); stack.push(3. * std::any_cast<long double>(op1));
            break;
        case INSTR::DIV:
            if(instruction.size() > 1) return WrongArity(file_name, line_num, "div", 0, instruction.size() - 1);
            op1 = stack.top(); stack.pop(); op2 = stack.top(); stack.pop();
            if(op2 == 0)
            {
                stack.push(op2); stack.push(op1);
                return DivisionByZero(file_name, line_num);
            }
            stack.push(std::any_cast<long double>(op1) / std::any_cast<long double>(op2));
            break;
       case INSTR::RDIV:
            if(instruction.size() > 1) return WrongArity(file_name, line_num, 0, instruction.size() - 1);
            if(std::any_cast<int>(stack.top()) == 0) return DivisionByZero(file_name, line_num);
            op1 = stack.top(); stack.pop(); op2 = stack.top(); stack.pop(); stack.push(std::any_cast<long double>(op2) / std::any_cast<long double>(op1));
            break;
       case INSTR::POW:
            if(instruction.size() > 1) return WrongArity(file_name, line_num, 0, instruction.size() - 1);
            op1 = stack.top(); stack.pop(); op2 = stack.top(); stack.pop(); stack.push(std::pow(std::any_cast<long double>(op1), std::any_cast<long double>(op2)));
            break;
       case INSTR::RPOW:
            if(instruction.size() > 1) return WrongArity(file_name, line_num, 0, instruction.size() - 1);
            op1 = stack.top(); stack.pop(); op2 = stack.top(); stack.pop(); stack.push(std::pow(std::any_cast<long double>(op2), std::any_cast<long double>(op1)));
            break;
       case INSTR::SQUARE:
            if(instruction.size() > 1) return WrongArity(file_name, line_num, 0, instruction.size() - 1);
            op1 = stack.top(); stack.pop(); stack.push(std::pow(std::any_cast<long double>(op1), 2));
            break;
       case INSTR::CUBE:
            if(instruction.size() > 1) return WrongArity(file_name, line_num, 0, instruction.size() - 1);
            op1 = stack.top(); stack.pop(); stack.push(std::pow(std::any_cast<long double>(op1), 3));
            break;
       case INSTR::ANDB:
            if(instruction.size() > 1) return WrongArity(file_name, line_num, 0, instruction.size() - 1);
            op1 = stack.top(); stack.pop(); op2 = stack.top(); stack.pop(); stack.push(std::any_cast<unsigned long long>(op1) & std::any_cast<unsigned long long>(op2));
            break;
       case INSTR::ORB:
            if(instruction.size() > 1) returnWrongArity(file_name, line_num, 0, instruction.size() - 1);
            op1 = stack.top(); stack.pop(); op2 = stack.top(); stack.pop(); stack.push(std::any_cast<unsigned long long>(op1) | std::any_cast<unsigned long long>(op2));
            break;
       // TODO: Bitwise operations and other operations
       case INSTR::PRINTI:
            if(instruction.size() > 1) return WrongArity(file_name, line_num, 0, instruction.size() - 1);
            std::cout << std::any_cast<int>(stack.top());
            break;
       case INSTR::PRINTPI:
            if(instruction.size() > 1) return WrongArity(file_name, line_num, 0, instruction.size() - 1);
            std::cout << std::any_cast<int>(stack.top()); stack.pop();
            break;
       case INSTR::SCANI:
            if(instruction.size() > 1) return WrongArity(file_name, line_num, 0, instruction.size() - 1);
            int x{}; std::cin >> x; stack.top() = x;
            break;
       case INSTR::IDXI:
            if(instruction.size() > 2) return WrongArity(file_name, line_num, 1, instruction.size() - 1);
            if(!is_array<decltype(stack.top())>) return OperandIsNotArray(file_name, line_num, stack.top());
            op1 = instruction[1]; stack.push(stack.top()[op1];
       default:
            return UndeclaredID(instruction[0]);
       }
    }
}
int main(int argc, char** argv)
{
    if(argc == 2) std::ifstream infile(argv[1]);
        if(IsFile(std::string(argv[1]))) return interpret(infile, std::string(argv[1]));
        else ExecuteCommand(std::string(argv[1]));
    else if(argc > 2)
    {
        std::vector<std::string> arr_argv{};
        for(auto i = 2; i < argc; i++) arr_argv.push_back(argv[i]);
        std::ifstream infile(argv[1]);
        return interpret(infile, std::string(argv[1]), arr_argv);
    }
    else return STATUS::NOFILE;
}
