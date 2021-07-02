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
#include<iostream>
#include<fstream>
//#include<any>
#include<string>
#include<vector>
#include<ctime>
#include<cmath>
#include "errors.hpp"
#include "tools.hpp"
#include "tokens.hpp"
#include "options.hpp"
#include "globals.hpp"
#include "sasm_interpreter.hpp"


// DO NOT TOUCH!!!
/*
// STATUS OperandIsNotArray(const std::string& file_name, const std::string& line, const std::size_t& line_num, const std::string& instr, const std::any& operand)
// {
//     std::cerr << "In file " << file_name << ": line: " << line_num << ": error: Operand '" << std::any_cast<std::string>(operand) << "' of instruction '" << instr << "' used in array context is not an array!" << std::endl;
//     std::cerr << line << std::endl;
//     return STATUS::ISNOTARRAY;
// }
*/
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
    time_t t = time(nullptr);
    std::cout << R"(SASM (Stack Assembly) language interpreter
Version: 0.1.0-beta+test
Author: Antoni Kiedos
Issue tracker: https://github.com/cpp-script-lang/sasm/issues
Contributing: https://github.com/cpp-script-lang/sasm/pulls
License: GNU GPLv3 or higher
--------------------------------------------------------------
Copyright (C) Antoni Kiedos 2021 - )" << (*localtime(&t)).tm_year + 1900 << ", Poland" << std::endl;
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
    if(file_name.size() <= 5) return false;
    else return file_name.substr(file_name.size() - 5) == ".sasm";
}
int main(int argc, char** argv)
{
    SasmInterpreter interp;
    if(argc == 2)
    {
        global_file_name = argv[1];
        if(IsValidFile(global_file_name))
        {
            std::ifstream infile(global_file_name);
            return static_cast<int>(interp.interpret(infile, global_file_name));
        }
        else return static_cast<int>(ExecuteCommand(global_file_name));
    }
    else if(argc > 2)
    {
        std::vector<std::string> arr_argv{};
        for(auto i = 2; i < argc; i++) arr_argv.push_back(argv[i]);
        std::ifstream infile(argv[1]);
        return static_cast<int>(interp.interpret(infile, std::string(argv[1]), arr_argv));
    }
    else return static_cast<int>(STATUS::NOFILE);
}
