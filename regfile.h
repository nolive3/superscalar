


#ifndef REGFILE_H
#define REGFILE_H
#include <cstdint>
struct reg {
    uint64_t tag;
    bool pending;
};
void reserve_reg(uint64_t reg, uint64_t tag);
void write_reg(uint64_t tag);
bool reg_busy(uint64_t regno);
uint64_t reg_tag(uint64_t regno);
#endif
