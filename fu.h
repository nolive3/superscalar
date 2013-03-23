

#ifndef FU_H
#define FU_H
#include <cstdint>
#include "procsim.hpp"
#include <iostream>
struct inst_entry{
    uint64_t tag = 0;
    bool filled = false;
};
template<int stages>
class FU{
    inst_entry buffer[stages+1];
    public:
        inst_entry completed(){
            return buffer[0];
        }
        void clock(){
            bool dc;
            if(buffer[stages-1+1].filled)
                getInstruction(buffer[stages-1+1].tag, dc).executed = cycle;
            for(int i = 1; i < stages+1; ++i){
                buffer[i-1] = buffer[i];
            }
            inst_entry nil;
            buffer[stages-1+1] = nil;
        }
        bool in_use(){
            return buffer[stages-1+1].filled;
        }
        bool use(inst_entry inst){
            if(in_use())
                return false;
            buffer[stages-1+1] = inst;
            return true;
        }
};
#endif
