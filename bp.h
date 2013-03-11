#ifndef BP_H
#define BP_H
#include <cstdint>
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
            if(taken){
                mcounter[ind] = min(mcounter[ind]+1, (1<<len)-1);
            }else{
                mcounter[ind] = max(mcounter[ind]-1, 0);
            }
            bool win = ((result == mtarget[ind]) && predt) || (!taken && !predt);
            if(taken){
                mtarget[ind] = result;
            }
            return win;
        }
    protected:
    private:
        uint32_t mtarget[mod];
        int mcounter[mod];

};

#endif // CACHE_H

