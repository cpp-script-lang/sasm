//
// Created by Antoni Kiedos on 30.06.2021.
//

#ifndef SASM_GLOBALS_HPP
#define SASM_GLOBALS_HPP
#include<stack>
#include<string>
#include<vector>
#include "tokens.hpp"

std::stack<std::string> global_stack{};
std::string global_line{};
std::string global_file_name{};
std::size_t global_line_num = 0;
std::vector<std::string> global_instruction{};
INSTR global_mnemonic{};
std::string global_op1{}, global_op2{}, global_op3{}, global_op4{};
int global_aux_i{};
std::stack<std::string> global_aux_dbg_stack{};
std::string global_aux_dbg_elem{};
bool global_printc_aux_b_operand_is_num{false};
int global_printc_aux_i{};
bool global_printpc_aux_b_operand_is_num{false};
int global_printpc_aux_i{};

#endif //SASM_GLOBALS_HPP
