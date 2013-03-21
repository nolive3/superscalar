#include "execute.h"

std::vector<FU<1>> fu0;
std::vector<FU<2>> fu1;
std::vector<FU<3>> fu2;
void execute(){
    for(auto i : fu0){
        i.clock();
    }
    for(auto i : fu1){
        i.clock();
    }
    for(auto i : fu2){
        i.clock();
    }
}
