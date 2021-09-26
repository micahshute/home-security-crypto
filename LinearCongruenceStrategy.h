#ifndef LINEAR_CONGRUENCE_STRATEGY
#define LINEAR_CONGRUENCE_STRATEGY

// NOTE: DO NOT USE (TODO BELOW)
// Use LehmerStrategy for non-cryptographic prng

namespace MSCrypto{
    template <typename T>
    class LinearCongruenceStrategy{
        private: 
            T addr;
            T mult; 
            T mod;
            const uint8_t SMALL_ADDR_MULTIPLIERS[2] = { 223, 7 };
            const uint16_t LARGE_ADDR_MULTIPLIERS[2] = { 4283, 997 };
        public: 
            LinearCongruenceStrategy(T addr, T mult, T mod);
            LinearCongruenceStrategy();
            T calculate(T previousNum);
            uint16_t revertFactor;
    };
};

template <typename T> 
MSCrypto::LinearCongruenceStrategy<T>::LinearCongruenceStrategy(){};

template <typename T>
MSCrypto::LinearCongruenceStrategy<T>::LinearCongruenceStrategy(T addr, T mult, T mod){
    this->addr = addr;
    this-> mult = mult;
    this->mod = mod;
    this->revertFactor = 1;
    if(mod > LARGE_ADDR_MULTIPLIERS[0]){
        addr = (T)LARGE_ADDR_MULTIPLIERS[0];
        mult = (T)LARGE_ADDR_MULTIPLIERS[1];
    }else{
        addr = (T)SMALL_ADDR_MULTIPLIERS[0];
        mult = (T)SMALL_ADDR_MULTIPLIERS[1];
    }
};

// TODO: FIX THIS: OVERFLOW HAPPENS
template <typename T>
T MSCrypto::LinearCongruenceStrategy<T>::calculate(T previousNum){
    T result = (((previousNum * mult) + addr) % mod);
    return result;
};


#endif