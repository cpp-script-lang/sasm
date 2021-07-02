//
// Created by Antoni Kiedos on 30.06.2021.
//

#ifndef SASM_SASM_INTERPRETER_HPP
#define SASM_SASM_INTERPRETER_HPP

#include "errors.hpp"
#include "options.hpp"
#include<tabulate/table.hpp>
#include<stack>
#include<algorithm>
#include<sstream>

extern std::stack<std::string> global_stack;
class SasmInterpreter
{
public:
    SasmInterpreter() = default;
    ~SasmInterpreter() = default;
    STATUS interpret(const std::ifstream& file, const std::string& file_name, const std::vector<std::string>& argv = {})
    {
        Options opts;
        tabulate::Table verbose_output;
        std::vector<tabulate::Table> line_tables{};
        //std::vector<tabulate::Table> mnemonic_tables{};
        if(!argv.empty()) if(argv[0] == "-v" || argv[0] == "--verbose") opts = Options(true);
        if(opts.verbose)
        {
            verbose_output.add_row({"FILE " + file_name}); // LOG
        }
        while(!file.eof())
        {
            global_line_num++;
            std::getline(const_cast<std::ifstream&>(file), global_line);
            if(opts.verbose)
            {
                if(!global_line.empty())
                {
                    line_tables.push_back(tabulate::Table{}.add_row({"LINE", global_line}).add_row({"NUMBER", std::to_string(global_line_num)})); // LOG
                }
            }
            global_instruction = Split(global_line);
            if(opts.verbose)
            {
                if(!global_line.empty())
                {
                    //mnemonic_tables.push_back(tabulate::Table{}.add_row({"MNEMONIC", }));
                    line_tables.back().add_row({"MNEMONIC", (global_instruction[0][0] == ';' ? "COMMENT" : global_instruction[0])}); // LOG
                }
            }
            global_mnemonic = GetInstrFromString(global_instruction[0]);
            switch(global_mnemonic)
            {
            case INSTR::LOAD:
                if(global_instruction.size() != 2) return WrongArity(file_name, global_line, global_line_num, "load", 1, global_instruction.size() - 1);
                global_stack.push(global_instruction[1]);
                if(opts.verbose)
                {
                    line_tables.back().add_row({"ARG", global_instruction[1]}); // LOG
                }
                break;
            case INSTR::POP:
                if(global_instruction.size() > 1) return WrongArity(file_name, global_line, global_line_num, "pop", 0, global_instruction.size() - 1);
                if(global_stack.empty()) return EmptyStack(file_name, global_line, global_line_num, "pop");
                global_stack.pop();
                break;
            case INSTR::POPX:
                if(global_instruction.size() != 2) return WrongArity(file_name, global_line, global_line_num, "popx", 1, global_instruction.size() - 1);
                global_op1 = global_instruction[1];
                for(auto i = 0; i < std::stoi(global_op1); i++)
                {
                    if(global_stack.empty()) return EmptyStack(file_name, global_line, global_line_num, "popx");
                    global_stack.pop();
                }
                if(opts.verbose)
                {
                    line_tables.back().add_row({"ARG", global_instruction[1]}); // LOG
                }
                break;
            case INSTR::INC:
                if(global_instruction.size() > 1) return WrongArity(file_name, global_line, global_line_num, "inc", 0, global_instruction.size() - 1);
                global_op1 = global_stack.top(); global_stack.pop(); global_stack.push(std::to_string(std::stoi(global_op1) + 1));
                break;
            case INSTR::DEC:
                if(global_instruction.size() > 1) return WrongArity(file_name, global_line, global_line_num, "dec", 0, global_instruction.size() - 1);
                global_op1 = global_stack.top(); global_stack.pop(); global_stack.push(std::to_string(std::stoi(global_op1) - 1));
                break;
            case INSTR::NEGI:
                if(global_instruction.size() > 1) return WrongArity(file_name, global_line, global_line_num, "negi", 0, global_instruction.size() - 1);
                global_op1 = global_stack.top(); global_stack.pop(); global_stack.push(std::to_string(-std::stoi(global_op1)));
                break;
            case INSTR::NEGF:
                if(global_instruction.size() > 1) return WrongArity(file_name, global_line, global_line_num, "negf", 0, global_instruction.size() - 1);
                global_op1 = global_stack.top(); global_stack.pop(); global_stack.push(std::to_string(-std::stof(global_op1)));
                break;
            case INSTR::ADDI:
                if(global_instruction.size() > 1) return WrongArity(file_name, global_line, global_line_num, "addi", 0, global_instruction.size() - 1);
                global_op1 = global_stack.top(); global_stack.pop(); global_op2 = global_stack.top(); global_stack.pop(); global_stack.push(std::to_string(std::stoi(global_op1) + std::stoi(global_op2)));
                break;
            case INSTR::ADDF:
                if(global_instruction.size() > 1) return WrongArity(file_name, global_line, global_line_num, "addf", 0, global_instruction.size() - 1);
                global_op1 = global_stack.top(); global_stack.pop(); global_op2 = global_stack.top(); global_stack.pop(); global_stack.push(std::to_string(std::stof(global_op1) + std::stof(global_op2)));
                break;
            case INSTR::SUBI:
                if(global_instruction.size() > 1) return WrongArity(file_name, global_line, global_line_num, "subi", 0, global_instruction.size() - 1);
                global_op1 = global_stack.top(); global_stack.pop(); global_op2 = global_stack.top(); global_stack.pop(); global_stack.push(std::to_string(std::stoi(global_op1) - std::stoi(global_op2)));
                break;
            case INSTR::SUBF:
                if(global_instruction.size() > 1) return WrongArity(file_name, global_line, global_line_num, "subf", 0, global_instruction.size() - 1);
                global_op1 = global_stack.top(); global_stack.pop(); global_op2 = global_stack.top(); global_stack.pop(); global_stack.push(std::to_string(std::stof(global_op1) - std::stof(global_op2)));
                break;
            case INSTR::RSUBI:
                if(global_instruction.size() > 1) return WrongArity(file_name, global_line, global_line_num, "rsubi", 0, global_instruction.size() - 1);
                global_op1 = global_stack.top(); global_stack.pop(); global_op2 = global_stack.top(); global_stack.pop(); global_stack.push(std::to_string(std::stoi(global_op2) - std::stoi(global_op1)));
                break;
            case INSTR::RSUBF:
                if(global_instruction.size() > 1) return WrongArity(file_name, global_line, global_line_num, "rsubf", 0, global_instruction.size() - 1);
                global_op1 = global_stack.top(); global_stack.pop(); global_op2 = global_stack.top(); global_stack.pop(); global_stack.push(std::to_string(std::stof(global_op2) - std::stof(global_op1)));
                break;
            case INSTR::MULI:
                if(global_instruction.size() > 1) return WrongArity(file_name, global_line, global_line_num, "muli", 0, global_instruction.size() - 1);
                global_op1 = global_stack.top(); global_stack.pop(); global_op2 = global_stack.top(); global_stack.pop(); global_stack.push(std::to_string(std::stoll(global_op1) * std::stoll(global_op2)));
                break;
            case INSTR::MULF:
                if(global_instruction.size() > 1) return WrongArity(file_name, global_line, global_line_num, "mulf", 0, global_instruction.size() - 1);
                global_op1 = global_stack.top(); global_stack.pop(); global_op2 = global_stack.top(); global_stack.pop(); global_stack.push(std::to_string(std::stold(global_op1) * std::stold(global_op2)));
                break;
            case INSTR::DOUBLEI:
                if(global_instruction.size() > 1) return WrongArity(file_name, global_line, global_line_num, "doublei", 0, global_instruction.size() - 1);
                global_op1 = global_stack.top(); global_stack.pop(); global_stack.push(std::to_string(2 * std::stoi(global_op1)));
                break;
            case INSTR::DOUBLEF:
                if(global_instruction.size() > 1) return WrongArity(file_name, global_line, global_line_num, "doublef", 0, global_instruction.size() - 1);
                global_op1 = global_stack.top(); global_stack.pop(); global_stack.push(std::to_string(2. * std::stof(global_op1)));
                break;
            case INSTR::TRIPLEI:
                if(global_instruction.size() > 1) return WrongArity(file_name, global_line, global_line_num, "triplei", 0, global_instruction.size() - 1);
                global_op1 = global_stack.top(); global_stack.pop(); global_stack.push(std::to_string(3 * std::stoi(global_op1)));
                break;
            case INSTR::TRIPLEF:
                if(global_instruction.size() > 1) return WrongArity(file_name, global_line, global_line_num, "triplef", 0, global_instruction.size() - 1);
                global_op1 = global_stack.top(); global_stack.pop(); global_stack.push(std::to_string(3. * std::stold(global_op1)));
                break;
            case INSTR::DIV:
                if(global_instruction.size() > 1) return WrongArity(file_name, global_line, global_line_num, "div", 0, global_instruction.size() - 1);
                global_op1 = global_stack.top(); global_stack.pop(); global_op2 = global_stack.top(); global_stack.pop();
                if(std::stoi(global_op2) == 0)
                {
                    global_stack.push(global_op2); global_stack.push(global_op1);
                    return DivisionByZero(file_name, global_line, global_line_num);
                }
                global_stack.push(std::to_string(std::stold(global_op1) / std::stold(global_op2)));
                break;
            case INSTR::RDIV:
                if(global_instruction.size() > 1) return WrongArity(file_name, global_line, global_line_num, "rdiv", 0, global_instruction.size() - 1);
                if(std::stoi(global_stack.top()) == 0)
                {
                    return DivisionByZero(file_name, global_line, global_line_num);
                }
                global_op1 = global_stack.top(); global_stack.pop(); global_op2 = global_stack.top(); global_stack.pop(); global_stack.push(std::to_string(std::stold(global_op2) / std::stold(global_op1)));
                break;
            case INSTR::POW:
                if(global_instruction.size() > 1) return WrongArity(file_name, global_line, global_line_num, "pow", 0, global_instruction.size() - 1);
                global_op1 = global_stack.top(); global_stack.pop(); global_op2 = global_stack.top(); global_stack.pop(); global_stack.push(std::to_string(std::pow(std::stold(global_op1), std::stold(global_op2))));
                break;
            case INSTR::RPOW:
                if(global_instruction.size() > 1) return WrongArity(file_name, global_line, global_line_num, "rpow", 0, global_instruction.size() - 1);
                global_op1 = global_stack.top(); global_stack.pop(); global_op2 = global_stack.top(); global_stack.pop(); global_stack.push(std::to_string(std::pow(std::stold(global_op2), std::stold(global_op1))));
                break;
            case INSTR::SQUARE:
                if(global_instruction.size() > 1) return WrongArity(file_name, global_line, global_line_num, "square", 0, global_instruction.size() - 1);
                global_op1 = global_stack.top(); global_stack.pop(); global_stack.push(std::to_string(std::pow(std::stold(global_op1), 2)));
                break;
            case INSTR::CUBE:
                if(global_instruction.size() > 1) return WrongArity(file_name, global_line, global_line_num, "cube", 0, global_instruction.size() - 1);
                global_op1 = global_stack.top(); global_stack.pop(); global_stack.push(std::to_string(std::pow(std::stold(global_op1), 3)));
                break;
            case INSTR::ANDB:
                if(global_instruction.size() > 1) return WrongArity(file_name, global_line, global_line_num, "andb", 0, global_instruction.size() - 1);
                global_op1 = global_stack.top(); global_stack.pop(); global_op2 = global_stack.top(); global_stack.pop(); global_stack.push(std::to_string(std::stoull(global_op1) & std::stoull(global_op2)));
                break;
            case INSTR::ORB:
                if(global_instruction.size() > 1) return WrongArity(file_name, global_line, global_line_num, "orb", 0, global_instruction.size() - 1);
                global_op1 = global_stack.top(); global_stack.pop(); global_op2 = global_stack.top(); global_stack.pop(); global_stack.push(std::to_string(std::stoull(global_op1) | std::stoull(global_op2)));
                break;
                // TODO: Bitwise operations and other operations
            case INSTR::PRINTI:
                if(global_instruction.size() > 1) return WrongArity(file_name, global_line, global_line_num, "printi", 0, global_instruction.size() - 1);
                std::cout << std::stoi(global_stack.top());
                break;
            case INSTR::PRINTC:
                if(global_instruction.size() > 1) return WrongArity(file_name, global_line, global_line_num, "printc", 0, global_instruction.size() - 1);
                try
                {
                    global_printc_aux_i = std::stoi(global_stack.top());
                    global_printc_aux_b_operand_is_num = true;
                }
                catch(...){}
                std::cout << (global_printc_aux_b_operand_is_num ? static_cast<char>(global_printc_aux_i) : global_stack.top()[0]);
                break;
            case INSTR::PRINTS:
                if(global_instruction.size() > 1) return WrongArity(file_name, global_line, global_line_num, "prints", 0, global_instruction.size() - 1);
                std::cout << global_stack.top();
                break;
            case INSTR::PRINTPI:
                if(global_instruction.size() > 1) return WrongArity(file_name, global_line, global_line_num, "printpi", 0, global_instruction.size() - 1);
                std::cout << std::stoi(global_stack.top()); global_stack.pop();
                break;
            case INSTR::PRINTPC:
                if(global_instruction.size() > 1) return WrongArity(file_name, global_line, global_line_num, "printpc", 0, global_instruction.size() - 1);
                try
                {
                    global_printpc_aux_i = std::stoi(global_stack.top());
                    global_printpc_aux_b_operand_is_num = true;
                }
                catch(...){}
                std::cout << (global_printpc_aux_b_operand_is_num ? static_cast<char>(global_printpc_aux_i) : global_stack.top()[0]); global_stack.pop();
                break;
            case INSTR::PRINTPS:
                if(global_instruction.size() > 1) return WrongArity(file_name, global_line, global_line_num, "printps", 0, global_instruction.size() - 1);
                std::cout << global_stack.top(); global_stack.pop();
                break;
            case INSTR::SCANI:
                if(global_instruction.size() > 1) return WrongArity(file_name, global_line, global_line_num, "scani", 0, global_instruction.size() - 1);
                std::cin >> global_aux_i; global_stack.push(std::to_string(global_aux_i));
                break;
                //    case INSTR::IDXI:
                //         if(instruction.size() > 2) return WrongArity(file_name, line, line_num, "idxi", 1, instruction.size() - 1);
                //         if(!is_array<decltype(stack.top())>::value) return OperandIsNotArray(file_name, line, line_num, instruction[0], stack.top());
                //         op1 = instruction[1]; stack.push(std::any_cast<std::vector<std::any>>(stack.top())[std::any_cast<std::size_t>(op1)]);
                //         break;
            case INSTR::DBG:
                if(global_instruction.size() > 1) return WrongArity(file_name, global_line, global_line_num, "dbg", 0, global_instruction.size() - 1);
                for(auto i = 0; i < global_stack.size(); i++)
                {
                    global_aux_dbg_stack = global_stack;
                    global_aux_dbg_elem = global_aux_dbg_stack.top(); global_aux_dbg_stack.pop();
                    std::cout << i << " | " << global_aux_dbg_elem << " |" << std::endl;
                }
                break;
            case INSTR::COMMENT:
                continue;
            case INSTR::EMPTYLINE:
                continue;
            default:
                return UndeclaredID(file_name, global_line, global_line_num, global_instruction[0]);
            }
            if(opts.verbose) verbose_output.add_row({line_tables.back( )});
        }
        if(opts.verbose) std::cout << verbose_output << std::endl;
        opts.log_file.close();
        if(!opts.log)
        {
#ifdef _WIN32
            system("del .\\dummyfile");
#else
            system("rm ./dummyfile");
#endif
        }
        return STATUS::SUCCESS;
    }
};


#endif