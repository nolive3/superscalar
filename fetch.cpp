#include "cache.h"
#include "bp.h"
#include "procsim.hpp"
#include "fetch.h"
#include "fetched.h"
#include "result_bus.h"

Cache<32, 12, 5> icache;
int stall_timer = 0;
const int STALL = 10;

uint64_t fetch_rate = 1;

uint64_t misprediction = 0;
bool mispredicted = false;

void fetch(proc_stats_t* stats){
    static uint64_t next = 1;
    static uint64_t progress = 0;
    static std::vector<uint64_t> fetching;
    if(stall_timer > 1){
        stall_timer--;
        stats->total_cache_stall++;
        return;
    }
    if(stall_timer == 1){
       // std::cerr << "Ending stall on cycle " << cycle << std::endl;
        stats->total_cache_stall++;
        stall_timer = -1;
    }
    if(stall_timer == 0){
        stall_timer = -1;
    }
    if(mispredicted&&!is_compleating(misprediction)){
       // std::cerr << "Stalling on instruction " << misprediction;
        stats->total_branch_stall++;
        return;
    } else {
        mispredicted = false;
    }
    for(uint64_t i = next+progress; i < next+fetch_rate; i++){;
        bool exists;
        proc_inst_t& inst = getInstruction(i, exists);
        if(!exists)
            break; //fetched all of the instructions
        if(!icache.access(inst.instruction_address)){
         //   std::cerr << "Starting stall on cycle " << cycle;
            stall_timer = STALL;// oops, cache miss
            break;
        }
        progress++;
        inst.predicted = true; // for later
        fetching.push_back(i);
        if(inst.op_code==-1){
            proc_inst_t inst2 = getInstruction(i+1, exists);
            proc_inst_t& inst3 = getInstruction(i, exists);
            if(exists){
                stats->num_branch++;
                inst3.predicted = predictor.predict(inst3.instruction_address, inst2.instruction_address);
                if(inst3.predicted){
                    stats->num_correct++;
                }else{
                    mispredicted = true;
                    misprediction = i;
                    if(i+1 < next+fetch_rate){
                        stats->total_branch_stall++;
                    }
                    break;
                }
            }
        }
    }
    for(uint64_t inst:fetching){
        bool dc;
        getInstruction(inst, dc).fetched = cycle;
        fetched.push_back(inst);
    }
    next+=progress;
    ifetched+=progress;
    fetching.clear();
    progress = 0;
}

bool was_cache_stalled(){
    return stall_timer >= 0 || mispredicted;
}
