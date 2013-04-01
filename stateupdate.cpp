#include "stateupdate.h"
#include "execute.h"
#include "result_bus.h"
#include "fu.h"
#include "procsim.hpp"
#include "regfile.h"
#include "bp.h"
#include "schedule.h"
#include "dispatch.h"
#include <algorithm>
#include <cstdint>

std::vector<unsigned long> stalls;

void clearRS(std::vector<inst_record>& rs, uint64_t tag);
void clearRS(std::vector<inst_record>& rs, uint64_t tag){
    for(inst_record& i:rs){
        if(i.instruction_number == tag){
            i.valid = false;
        }
    }
}

std::vector<inst_entry> retiring;

void state_update2(){
    for(inst_entry& i : retiring){
        clearRS(rs0,i.tag);
        clearRS(rs1,i.tag);
        clearRS(rs2,i.tag);
    }
}

void state_update(){
    clear_busses();
    retiring.clear();
    for(auto& i : fu0){
        inst_entry t = i.completed();
        if(t.filled)
            retiring.push_back(t);
    }
    for(auto& i : fu1){
        inst_entry t = i.completed();
        if(t.filled)
            retiring.push_back(t);
    }
    for(auto& i : fu2){
        inst_entry t = i.completed();
        if(t.filled)
            retiring.push_back(t);
    }
    sort(begin(retiring), end(retiring), [](inst_entry i, inst_entry j){return i.tag<j.tag;});

    for(inst_entry& i : retiring){
        write_reg(i.tag);
        complete_instruction(i.tag);
        icompleated++;
        bool dc;
        proc_inst_t & inst = getInstruction(i.tag,dc);
        proc_inst_t next = getInstruction(i.tag+1, dc);
        inst.retired = cycle;
        if(inst.op_code==-1&&dc){
            predictor.update(inst.instruction_address, next.instruction_address);
        }
    }
}
