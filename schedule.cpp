#include "schedule.h"
#include "result_bus.h"
#include "execute.h"

std::vector<inst_record> rs0;
std::vector<inst_record> rs1;
std::vector<inst_record> rs2;

void schedule(){
    for (inst_record& i : rs0){
        if(!i.reg_ready[0]){
            i.reg_ready[0] = is_compleating(i.reg_deps[0]);
        }
        if(!i.reg_ready[1]){
            i.reg_ready[1] = is_compleating(i.reg_deps[1]);
        }
        if(i.reg_ready[0]&&i.reg_ready[1]&&!i.running){
            inst_entry t;
            t.filled = true;
            t.tag = i.instruction_number;
            for(auto & fu : fu0){
                if(fu.use(t)){
                    i.running = true;
                    break;
                }
            }
        }
    }
    for (inst_record& i : rs1){
        if(!i.reg_ready[0]){
            i.reg_ready[0] = is_compleating(i.reg_deps[0]);
        }
        if(!i.reg_ready[1]){
            i.reg_ready[1] = is_compleating(i.reg_deps[1]);
        }
        if(i.reg_ready[0]&&i.reg_ready[1]&&!i.running){
            inst_entry t;
            t.filled = true;
            t.tag = i.instruction_number;
            for(auto & fu : fu1){
                if(fu.use(t)){
                    i.running = true;
                    break;
                }
            }
        }
    }
    for (inst_record& i : rs1){
        if(!i.reg_ready[0]){
            i.reg_ready[0] = is_compleating(i.reg_deps[0]);
        }
        if(!i.reg_ready[1]){
            i.reg_ready[1] = is_compleating(i.reg_deps[1]);
        }
        if(i.reg_ready[0]&&i.reg_ready[1]&&!i.running){
            inst_entry t;
            t.filled = true;
            t.tag = i.instruction_number;
            for(auto & fu : fu1){
                if(fu.use(t)){
                    i.running = true;
                    break;
                }
            }
        }
    }
}
