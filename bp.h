#ifndef BP_H
#define BP_H
#include <cstdint>
#include <iostream>
#include <iomanip>
#include "procsim.hpp"

template <int len, int div, int mod>
class BP
{
    friend std::ostream& operator<<(std::ostream& out, const BP& t){
        out << len << " (N/" << div << ")%" << mod << ", Contents:" << std::endl;
        for (int i = 0; i < mod; ++i){
            out << i << ": (0x" << std::hex << std::setw(((32)>>2)) << std::setfill('0') << t.mtarget[i] << ", ";
            out << t. mcounter[i] << ")" << std::endl;
        }
        return out;
    }
    public:
        BP(){
            for(int i = 0; i < mod; ++i){
                mtarget[i] = 0;
                mcounter[i] = (1<<(len-1))-1;
            }
        }

        bool predict(uint32_t branch, uint32_t result){
		
            int ind = (branch/div)%mod;
            bool taken = result != branch+4;
            bool predt = mcounter[ind] >= 1<<(len-1);
            bool win = ((result == mtarget[ind]) && predt) || (!taken && !predt);
            /*
                std::cerr << std::dec << cycle << ": Checking index " << ind << ": (0x" << std::hex << std::setw(((32)>>2)) << std::setfill('0') << mtarget[ind] << ", ";
                std::cerr << mcounter[ind] << ") ";
                std::cerr << std::boolalpha << "Taken? " << taken << " Prediction " << predt << std::hex << std::setw(((32)>>2)) << std::setfill('0') << branch << "->" << result << std::endl;
            */
            return win;
        }

        void update(uint32_t branch, uint32_t result){
            bool taken = result != branch+4;
            int ind = (branch/div)%mod;
            if(taken){
                mcounter[ind] = std::min(mcounter[ind]+1, (1<<len)-1);
            }else{
                mcounter[ind] = std::max(mcounter[ind]-1, 0);
            }
            if(taken){
                mtarget[ind] = result;
            }
            /*
                std::cerr << std::dec << cycle << ": Updating index " << ind << ": (0x" << std::hex << std::setw(((32)>>2)) << std::setfill('0') << mtarget[ind] << ", ";
                std::cerr << mcounter[ind] << ") ";
                std::cerr << std::boolalpha << "Taken? " << taken << " Prediction " << std::hex << std::setw(((32)>>2)) << std::setfill('0') << branch << "->" << result << std::endl;
            */
        }
    protected:
    private:
        uint32_t mtarget[mod];
        int mcounter[mod];

};


extern BP<2, 8, 512> predictor;

#endif // CACHE_H

