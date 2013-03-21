#include "cache.h"
#include "bp.h"
#include "procsim.h"
#include "fetch.h"
#include "fetched.h"

Cache<32, 12, 5> icache;
int stall_timer = 0;
const int STALL = 10;

uint64_t fetch_rate = 1;



void fetch(proc_stats_t* stats){
    static uint64_t next = 1;
    static uint64_t progress = 0;
    static vector<uint64_t> fetching;
    if(stall_timer > 0){
        stall_timer--;
        stats->total_cache_stall++;
        return;
    }
    if(stall_timer <= 0){
        stall_timer = -1;
    }
    for(uint64_t i = next+progress; i < next+fetch_rate; i++){
        progress++;
        fetching.push_back(i);
        bool exists;
        proc_inst_t& inst = getInstruction(i, exists);
        if(!exists)
            break; //fetched all of the instructions
        proc_inst_t& inst2 = getInstruction(i, exists);
        if(inst.op_code==-1&&exists){
            stats->num_branch++;
            inst.predicted = predictor.predict(inst.instruction_address, inst2.instruction_address);
            if(inst.predicted){
                stats->num_correct++;
            }
        }
        if(icache.access(inst.instruction_address)){

        } else {
            stall_timer = STALL;// oops, cache miss
            return;
        }
    }
    progress = 0;
}

bool was_cache_stalled(){
    return stall_timer >= 0;
}
