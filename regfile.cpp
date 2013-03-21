#include "regfile.h"
#include <vector>

std::vector<reg> regfile;
reg& get_register(uint64_t regno);
reg& get_register(uint64_t regno){
    while(regfile.size()<=regno){
        reg nil = {0, false};
        regfile.push_back(nil);
    }
    return regfile[regno];
}


void reserve_reg(uint64_t regno, uint64_t tag){
    reg& r = get_register(regno);
    r.pending = true;
    r.tag = tag;
}
void write_reg(uint64_t tag){
    for(reg& r : regfile)
    	if(r.tag == tag)
        	r.pending = false;
}
bool reg_busy(uint64_t regno){
    return get_register(regno).pending;
}
uint64_t reg_tag(uint64_t regno){
    return get_register(regno).tag;
}
