#ifndef SASM_TOOLS_HPP
#define SASM_TOOLS_HPP
#include<type_traits>
#include<vector>
#include<string>
#include<sstream>
#include<regex>
#include "tokens.hpp"
enum class INSTR;
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
    else if(instr == "PRINTS" || instr == "prints") return INSTR::PRINTS;
    else if(instr == "PRINTB" || instr == "printb") return INSTR::PRINTB;
    else if(instr == "PRINTF" || instr == "printf") return INSTR::PRINTF;
    else if(instr == "PRINTPI" || instr == "printpi") return INSTR::PRINTPI;
    else if(instr == "PRINTPC" || instr == "printpc") return INSTR::PRINTPC;
    else if(instr == "PRINTPS" || instr == "printps") return INSTR::PRINTPS;
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
    else if(std::regex_match(instr, std::regex(R"(;.*?)"))) return INSTR::COMMENT;
    else if(instr.empty()) return INSTR::EMPTYLINE;
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
    if(str.empty()) return {""};
    std::vector<std::string> words{};
    std::istringstream ss(str);
    std::string token;
    std::getline(ss, token, ' ');
    words.push_back(token);
    //std::clog << "Current token: " << token << std::endl;
    while(std::getline(ss, token, ','))
    {
        while(token[0] == ' ') token.erase(0);
        words.push_back(token);
        //std::clog << "Current token: " << token << std::endl;
    }
    return words;
}
#endif