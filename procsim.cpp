#include "procsim.hpp"
#include "cache.h"
#include <iostream>
#include <vector>

#include "fetch.h"
#include "fetched.h"
#include "dispatch.h"
#include "schedule.h"
#include "execute.h"
#include "stateupdate.h"

uint64_t ifetched = 0;
uint64_t icompleated = 0;
uint64_t cycle = 0;
std::vector<proc_inst_t> instruction_list;

proc_inst_t& getInstruction(uint64_t ino, bool& success){
    while(instruction_list.size()<ino){
        proc_inst_t tmp;
        if(read_instruction(&tmp)){
            instruction_list.push_back(tmp);
        } else {
            success = false;
            return instruction_list[0];
        }
    }
    success = true;
    return instruction_list[ino-1]; // instructions are 1 indexed, the vector is 0 indexed.
}

/**
 * Subroutine for initializing the processor. You may add and initialize any global or heap
 * variables as needed.
 * XXX: You're responsible for completing this routine
 *
 * @k0 Number of k0 FUs
 * @k1 Number of k1 FUs
 * @k2 Number of k2 FUs
 * @f Number of instructions to fetch
 * @m Schedule queue multiplier
 */
void setup_proc(uint64_t k0, uint64_t k1, uint64_t k2, uint64_t f, uint64_t m) {
    fetch_rate = f;
    fu0.resize(k0);
    rs0.resize(m*k0);
    fu1.resize(k1);
    rs1.resize(m*k1);
    fu2.resize(k2);
    rs2.resize(m*k2);
}

/**
 * Subroutine that simulates the processor.
 *   The processor should fetch instructions as appropriate, until all instructions have executed
 * XXX: You're responsible for completing this routine
 *
 * @p_stats Pointer to the statistics structure
 */
void run_proc(proc_stats_t* p_stats) {
    while (ifetched>icompleated || was_cache_stalled()){
        state_update(p_stats);
        execute();
        schedule();
        dispatch(p_stats);
        fetch(p_stats);
        p_stats->total_dqueue_size += fetched.size();
        cycle++;
    }
}

/**
 * Subroutine for cleaning up any outstanding instructions and calculating overall statistics
 * such as average IPC or branch prediction percentage
 * XXX: You're responsible for completing this routine
 *
 * @p_stats Pointer to the statistics structure
 */
void complete_proc(proc_stats_t *p_stats) {
    cycle += p_stats->total_branch_stall;
    p_stats->avg_dqueue_size = p_stats->total_dqueue_size/(float)cycle;
    p_stats->avg_inst_fire = ifetched/(float)cycle;
    p_stats->avg_ipc = icompleated/(float)cycle;
    p_stats->cycle_count = cycle;
    p_stats->retired_instruction = icompleated;
}
