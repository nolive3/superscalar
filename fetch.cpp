#include "cache.h"
#include "bp.h"
#include "procsim.hpp"
#include "fetch.h"
#include "fetched.h"

Cache<32, 12, 5> icache;
int stall_timer = 0;
const int STALL = 10;

uint64_t fetch_rate = 1;



void fetch(proc_stats_t* stats){
    static uint64_t next = 1;
    static uint64_t progress = 0;
    static std::vector<uint64_t> fetching;
    if(stall_timer > 0){
        stall_timer--;
        stats->total_cache_stall++;
        return;
    }
    if(stall_timer <= 0){
        stall_timer = -1;
    }
    for(uint64_t i = next+progress; i < next+fetch_rate; i++){;
        bool exists;
        proc_inst_t& inst = getInstruction(i, exists);
        if(!exists)
            break; //fetched all of the instructions
        if(!icache.access(inst.instruction_address)){
            stall_timer = STALL;// oops, cache miss
            break;
        }
        progress++;
        fetching.push_back(i);
        proc_inst_t& inst2 = getInstruction(i+1, exists);
        if(exists&&inst.op_code==-1){
            stats->num_branch++;
            inst.predicted = predictor.predict(inst.instruction_address, inst2.instruction_address);
            if(inst.predicted){
                stats->num_correct++;
            }
        } else {
            inst.predicted = true; // for later
        }
    }
    for(uint64_t inst:fetching){
        bool dc;
        getInstruction(inst, dc).fetched = cycle-1;
        fetched.push_back(inst);
    }
    next+=progress;
    ifetched+=progress;
    fetching.clear();
    progress = 0;
}

bool was_cache_stalled(){
    return stall_timer >= 0;
}
