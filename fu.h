

#ifndef FU_H
#define FU_H
#include <cstdint>
#include "procsim.hpp"
struct inst_entry{
    uint64_t tag = 0;
    bool filled = false;
};
template<int stages>
class FU{
    inst_entry buffer[stages];
    public:
        inst_entry completed(){
            return buffer[0];
        }
        void clock(){
            bool dc;
            getInstruction(buffer[stages-1].tag, dc).executed = cycle;
            for(int i = 1; i < stages; ++i){
                buffer[i-1] = buffer[i];
            }
            inst_entry nil;
            buffer[stages-1] = nil;
        }
        bool in_use(){
            return buffer[stages-1].filled;
        }
        bool use(inst_entry inst){
            if(in_use())
                return false;
            buffer[stages-1] = inst;
            return true;
        }
};
#endif
