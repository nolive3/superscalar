#ifndef CACHE_H
#define CACHE_H
#include <cstdint>
#include <iostream>
#include <iomanip>
template <int addr_len, int C, int B>
class Cache
{
    friend std::ostream& operator<<(std::ostream& out, const Cache& t){
        out << addr_len << " (" << C << ", " << B << ", 0) Contents:" << std::endl;
        for (int i = 0; i < 1<<(C-B); ++i){
            out << i << ": (0x" << std::hex << std::setw(((addr_len-C+2)>>2)) << std::setfill('0') << t.mtag[i] << ", ";
            out << std::boolalpha << t. mvalid[i] << ")" << std::endl;
        }
        return out;
    }
    public:
        Cache() {
            for (int i = 0; i < 1<<(C-B); ++i){
                mvalid[i] = false;
                mtag[i] = 0;
            }
        }
        bool access(uint32_t addr){
            if(mvalid[index(addr)] && mtag[index(addr)] == tag(addr)){
                return true;
            }
            mvalid[index(addr)] = true;
            mtag[index(addr)] = tag(addr);
            return false;
        }
    private:
        uint32_t index(uint32_t addr){return (addr>>B)&((1<<(C-B))-1);}
        uint32_t tag(uint32_t addr){return (addr>>C)&((1<<(addr_len-C))-1);}
        int mtag[1<<(C-B)];
        bool mvalid[1<<(C-B)];
};

#endif // CACHE_H
