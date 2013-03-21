#ifndef FETCH_H
#define FETCH_H
extern uint64_t fetch_rate;
void fetch(proc_stats_t* stats);
bool was_cache_stalled();
#endif
