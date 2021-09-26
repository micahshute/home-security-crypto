#include "LehmerStrategy.h"

MSCrypto::LehmerStrategy::LehmerStrategy(){};

MSCrypto::LehmerStrategy::LehmerStrategy(uint32_t seed){
    this->mult = 48271;
    this->mod = 2147483647;
    this->seed = seed;
    this->n_m_1 = seed;
};

uint32_t MSCrypto::LehmerStrategy::calculate(){
    uint64_t prod = (uint64_t)this->n_m_1 * (uint64_t)(this->mult);
    uint32_t result = prod % (uint64_t)mod;
    this->n_m_1 = result;
    return result;
};

void MSCrypto::LehmerStrategy::revert(uint32_t state){
    this->n_m_1 = state;
}

void MSCrypto::LehmerStrategy::reset(){
    this->n_m_1 = this->seed;
}









// 32-bit arithmetic when mult = 16807

    // this->q = 127773;
    // this->r = 2836;

    // uint32_t hi = this->n_m_1 / q;
    // uint32_t lo = this->n_m_1 % q;
    // int res = (this->mult * lo) - (this->r * hi);
    // if(res <= 0){
    //     res += this->mod;
    // }
    // this->n_m_1 = res;
    // return (uint32_t)res;
