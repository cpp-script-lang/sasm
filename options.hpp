//
// Created by Antoni Kiedos on 30.06.2021.
//

#ifndef SASM_OPTIONS_HPP
#define SASM_OPTIONS_HPP
struct Options
{
    bool verbose;
    bool log;
    std::ofstream log_file;
    explicit Options(const bool& verbose = false, const bool& log = false, const std::string& log_file = std::string())
    {
        this->verbose = verbose;
        this->log = log;
        if(log && !log_file.empty()) this->log_file.open(log_file);
        else if(log) this->log_file.open("sasm_log.log");
        else this->log_file.open("dummyfile");
    }
};
#endif //SASM_OPTIONS_HPP
