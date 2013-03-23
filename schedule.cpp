#include "schedule.h"
#include "result_bus.h"
#include "execute.h"
#include <algorithm>

std::vector<inst_record> rs0;
std::vector<inst_record> rs1;
std::vector<inst_record> rs2;

void schedule(){
    sort(begin(rs0), end(rs0), [](inst_record i, inst_record j){return i.instruction_number<j.instruction_number;});
    sort(begin(rs1), end(rs1), [](inst_record i, inst_record j){return i.instruction_number<j.instruction_number;});
    sort(begin(rs2), end(rs2), [](inst_record i, inst_record j){return i.instruction_number<j.instruction_number;});
   /* std::cerr << "RS0";
        for (auto i : rs0)
            std::cerr << " -> " << i.instruction_number << " " << std::boolalpha << i.valid;
    std::cerr << std::endl;
    std::cerr << "RS1";
        for (auto i : rs1)
            std::cerr << " -> " << i.instruction_number << " " << std::boolalpha << i.valid;
    std::cerr << std::endl;
    std::cerr << "RS2";
        for (auto i : rs2)
            std::cerr << " -> " << i.instruction_number << " " << std::boolalpha << i.valid;
    std::cerr << std::endl;*/
    for (inst_record& i : rs0){
        if(!i.valid)
		continue;
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
        if(!i.reg_ready[0]){
            i.reg_ready[0] = is_compleating(i.reg_deps[0]);
        }
        if(!i.reg_ready[1]){
            i.reg_ready[1] = is_compleating(i.reg_deps[1]);
        }
    }
    for (inst_record& i : rs1){
        if(!i.valid)
		continue;
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
        if(!i.reg_ready[0]){
            i.reg_ready[0] = is_compleating(i.reg_deps[0]);
        }
        if(!i.reg_ready[1]){
            i.reg_ready[1] = is_compleating(i.reg_deps[1]);
        }
    }
    for (inst_record& i : rs2){
        if(!i.valid)
		continue;
        if(i.reg_ready[0]&&i.reg_ready[1]&&!i.running){
            inst_entry t;
            t.filled = true;
            t.tag = i.instruction_number;
            for(auto & fu : fu2){
                if(fu.use(t)){
                    i.running = true;
                    break;
                }
            }
        }
        if(!i.reg_ready[0]){
            i.reg_ready[0] = is_compleating(i.reg_deps[0]);
        }
        if(!i.reg_ready[1]){
            i.reg_ready[1] = is_compleating(i.reg_deps[1]);
        }
    }
}
