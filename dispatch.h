
#ifndef DISPATCH_H
#define DISPATCH_H
#include <cstdint>
#include "procsim.hpp"
struct inst_record{
    uint64_t instruction_number = 0;
    bool reg_ready[2] = {false,false};
    uint64_t reg_deps[2] = {0,0};
    bool valid = false;
    bool running = false;
};

void dispatch(proc_stats_t* stats);
#endif
