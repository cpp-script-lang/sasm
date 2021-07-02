#pragma once
#include<functional>
#include<iostream>

extern std::string global_file_name, global_line;
extern std::size_t global_line_num;
enum class STATUS
{
    UNRECOGNISEDARG = -390, ARGUMENTSOVERFLOW = 3, ARGUMENTSUNDERFLOW = -3, DIVBYZERO = -1,
    SUCCESS = 0, /*ISNOTARRAY = 1,*/ UNDECLAREDID = 2, NOFILE = -2, EMPTYSTACK = 390
};

template<typename ...AdditionalParams>
STATUS Error(const std::function<STATUS(const std::string&, const std::string&, const std::size_t&, AdditionalParams...)>& f, AdditionalParams ...args)
{
    return f(global_file_name, global_line, global_line_num, args...);
}
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
STATUS EmptyStack(const std::string& file_name, const std::string& line, const std::size_t& line_num, const std::string& instr)
{
    std::cerr << "In file " << file_name << ": line: " << line_num << ": error: Stack is empty, but '" << instr << "' was used!" << std::endl;
    std::cerr << line << std::endl;
    return STATUS::EMPTYSTACK;
}
STATUS DivisionByZero(const std::string& file_name, const std::string& line, const std::size_t& line_num)
{
    std::cerr << "In file " << file_name << ": line: " << line_num << ": error: Division by 0!" << std::endl;
    std::cerr << line << std::endl;
    return STATUS::DIVBYZERO;
}