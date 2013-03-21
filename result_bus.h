#ifndef RESULT_BUS_H
#define RESULT_BUS_H

#include <cstdint>

uint64_t num_busses();
void clear_busses();
bool is_compleating(uint64_t tag);
void complete_instruction(uint64_t tag);
#endif
