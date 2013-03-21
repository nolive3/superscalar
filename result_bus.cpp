#include "result_bus.h"
#include <vector>

std::vector<uint64_t> busses;



uint64_t num_busses(){
    return busses.size();
}
void clear_busses(){
    busses.clear();
}
bool is_compleating(uint64_t tag){
    for(uint64_t& i : busses){
        if(tag == i)
            return true;
    }
    return false;
}
void complete_instruction(uint64_t tag){
    busses.push_back(tag);
}
