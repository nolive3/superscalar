#ifndef CACHE_H
#define CACHE_H
#include <cstdint>
template <int addr_len, int C, int B>
class cache
{
    public:
        bool access(uint32_t addr){
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
        int mtag[1<<(C-B)];
        bool mvalid[1<<(C-B)];

};

#endif // CACHE_H
