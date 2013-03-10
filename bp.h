#ifndef BP_H
#define BP_H
#include <cstdint>
template <int bits, int div, int mod>
class BP
{
    public:
        bool predict(uint32_t branch, uint32_t result){
            if(mvalid[index(addr)] && mtag[index(addr)] == tag(addr)){
                return true;
            }
            mvalid[index(addr)] = true;
            mtag[index(addr)] = tag(addr);
            return false;
        }

    protected:
    private:
        uint32_t index(uint32_t addr){return (addr>>B)&((1<<(C-B))-1);}
        uint32_t tag(uint32_t addr){return (addr>>C)&((1<<(addr_len-C))-1);}
        uint32_t mtarget[mod];
        bool mvalid[1<<(C-B)];

};

#endif // CACHE_H

