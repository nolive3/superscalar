#include <cstdio>
#include <cinttypes>
#include <cstdlib>
#include <cstring>
#include <unistd.h>
#include <iostream>
#include "procsim.hpp"
#include "cache.h"
#include "bp.h"

using namespace std;

void test(void);
void test(void)
{
    uint64_t num_branch = 0;
    uint64_t num_correct = 0;
	const int N = 10;
	proc_inst_t ins[N];
	read_instruction(ins+1);
	while(read_instruction(ins)){
		if(ins[1].op_code == -1){
			num_branch++;
			if(predictor.predict(ins[1].instruction_address, ins[0].instruction_address))
				num_correct++;
		}
		if(ins[N-1].op_code == -1)
			predictor.update(ins[N-1].instruction_address, ins[N-2].instruction_address);

		for(int i = N-2; i >=0; --i){
			ins[i+1] = ins[i];
		}
	}
	printf("Percentage Correct Branch Predictions: %f\n", 100*(num_correct/(float)num_branch));
}


void print_help_and_exit(void);
void print_help_and_exit(void) {
    printf("procsim [OPTIONS] < traces/file.trace\n");
    printf("  -j k0\t\tNumber of k0 FUs\n");
    printf("  -k k1\t\tNumber of k1 FUs\n");
    printf("  -l k2\t\tNumber of k2 FUs\n");
    printf("  -m M\t\tNumber of blocks per set is 2^S\n");
    printf("  -f N\t\tNumber of instructions to fetch\n");
    printf("  -t\trun test function\n");
    printf("  -h\t\tThis helpful output\n");
    exit(0);
}

//
// read_instruction
//
//  returns true if an instruction was read successfully
//
bool read_instruction(proc_inst_t* p_inst)
{
    int ret;

    if (p_inst == NULL)
    {
        fprintf(stderr, "Fetch requires a valid pointer to populate\n");
        return false;
    }

    ret = fscanf(stdin, "%x %d %d %d %d\n", &p_inst->instruction_address,
                 &p_inst->op_code, &p_inst->dest_reg, &p_inst->src_reg[0], &p_inst->src_reg[1]);
    if (ret != 5) {
        return false;
    }

    return true;
}

void print_statistics(proc_stats_t* p_stats);

int main(int argc, char* argv[]) {
    int opt;
    uint64_t f = DEFAULT_F;
    uint64_t m = DEFAULT_M;
    uint64_t k0 = DEFAULT_K0;
    uint64_t k1 = DEFAULT_K1;
    uint64_t k2 = DEFAULT_K2;

    /* Read arguments */
    while(-1 != (opt = getopt(argc, argv, "j:k:l:f:m:h:t"))) {
        switch(opt) {
        case 'j':
            k0 = atoi(optarg);
            break;
        case 'k':
            k1 = atoi(optarg);
            break;
        case 'l':
            k2 = atoi(optarg);
            break;
        case 'm':
            m = atoi(optarg);
            break;
        case 'f':
            f = atoi(optarg);
            break;
        case 't':
            test();
            exit(0);
        case 'h':
            /* Fall through */
        default:
            print_help_and_exit();
            break;
        }
    }


    /* Setup the cache */
    setup_proc(k0, k1, k2, f, m);

    /* Setup statistics */
    proc_stats_t stats;
    memset(&stats, 0, sizeof(proc_stats_t));

    /* Run the processor */
    run_proc(&stats);

    /* Finalize stats */
    complete_proc(&stats);

    print_statistics(&stats);

    return 0;
}

void print_statistics(proc_stats_t* p_stats) {
    printf("Processor stats:\n");
	printf("Avg inst fired per cycle: %f\n", p_stats->avg_inst_fire);
	printf("Total instructions: %lu\n", p_stats->retired_instruction);
	printf("Total run time (cycles): %lu\n", p_stats->cycle_count);
	printf("Avg IPC: %f\n", p_stats->avg_ipc);
    printf("Maximum Dispatch Queue: %lu\n", p_stats->max_dqueue_size);
    printf("Average Dispatch Queue: %f\n", p_stats->avg_dqueue_size);
    printf("Percentage Correct Branch Predictions: %f(%llu/%llu)\n", 100*(p_stats->num_correct/(float)p_stats->num_branch), p_stats->num_correct, p_stats->num_branch);
    printf("Cycles stalled on ICache: %lu\n", p_stats->total_cache_stall);
    printf("Cycles stalled on Branch Mispredict: %lu\n", p_stats->total_branch_stall);
}

