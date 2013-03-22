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

void clearRS(std::vector<inst_record>& rs, uint64_t tag);
void clearRS(std::vector<inst_record>& rs, uint64_t tag){
    for(inst_record& i:rs){
        if(i.instruction_number == tag){
            i.valid = false;
        }
    }
}

void state_update(proc_stats_t* stats){
    clear_busses();
    std::vector<inst_entry> retiring;
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
        bool dc;
        proc_inst_t & inst = getInstruction(i.tag,dc);
        proc_inst_t next = getInstruction(i.tag+1, dc);
        inst.fetched += stats->total_branch_stall;
        inst.executed += stats->total_branch_stall;
        inst.dispatched += stats->total_branch_stall;
        inst.scheduled += stats->total_branch_stall;
        inst.retired = cycle+stats->total_branch_stall;
        if(inst.op_code==-1&&dc){
            predictor.update(inst.instruction_address, next.instruction_address);
        }
        if(!inst.predicted){
            int delta = inst.retired-inst.fetched-1;
            stats->total_branch_stall+=delta;
        }
        clearRS(rs0,i.tag);
        clearRS(rs1,i.tag);
        clearRS(rs2,i.tag);
	write_reg(i.tag);
	complete_instruction(i.tag);
	icompleated++;
    }
}
