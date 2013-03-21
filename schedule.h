
#ifndef SCHEDULE_H
#define SCHEDULE_H
#include "procsim.hpp"
#include "dispatch.h"
#include <vector>
extern std::vector<inst_record> rs0;
extern std::vector<inst_record> rs1;
extern std::vector<inst_record> rs2;
void schedule();
#endif
