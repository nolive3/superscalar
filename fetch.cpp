#include "cache.h"
#include "bp.h"
#include "procsim.h"
#include "fetched.h"

Cache<32, 12, 5> icache;
int stall_timer = 0;
const int STALL = 10;


void fetch(proc_stats_t* stats, uint64_t max_num){
    if(stall_timer > 0){
        stall_timer--;
        stats->total_cache_stall++;
        return;
    }

}

bool is_cache_stalled(){
    return stall_timer > 0;
}
