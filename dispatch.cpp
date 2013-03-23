#include "dispatch.h"
#include "fetched.h"
#include "regfile.h"
#include "schedule.h"
#include "result_bus.h"
#include <iostream>


void dispatch(proc_stats_t* stats){
    bool success = true;
    while (success&&fetched.size()>0){
        uint64_t inst = fetched[0];
        bool exists;
        proc_inst_t& realinst = getInstruction(inst, exists);
        inst_record i;
        i.instruction_number = inst;
        i.valid = true;
	i.reg_ready[0] = true;
	i.reg_ready[1] = true;
        if(realinst.src_reg[0] != -1){
            i.reg_ready[0] = !reg_busy(realinst.src_reg[0]) || is_compleating(reg_tag(realinst.src_reg[0]));
            i.reg_deps[0] = reg_tag(realinst.src_reg[0]);
        }
        if(realinst.src_reg[1] != -1){
            i.reg_ready[1] = !reg_busy(realinst.src_reg[1]) || is_compleating(reg_tag(realinst.src_reg[1]));
            i.reg_deps[1] = reg_tag(realinst.src_reg[1]);
        }
        success = false;
        switch(realinst.op_code){
        case -1:
        case 0:
            for(auto & record : rs0){
                if(!record.valid){
                    record = i;
                    success = true;
                    break;
                }
            }
            break;
        case 1:
            for(auto & record : rs1){
                if(!record.valid){
                    record = i;
                    success = true;
                    break;
                }
            }
            break;
        case 2:
            for(auto & record : rs2){
                if(!record.valid){
                    record = i;
                    success = true;
                    break;
                }
            }
            break;
        default:
            //wtf
            break;
        }
        if(success){
            realinst.scheduled = cycle+1;
            if(realinst.dest_reg != -1)
                reserve_reg(realinst.dest_reg, inst);
            if(fetched.size() > stats->max_dqueue_size)
                stats->max_dqueue_size = fetched.size();
            fetched.erase(begin(fetched));
        }
    }
}
