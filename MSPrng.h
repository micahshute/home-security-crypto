#ifndef MS_PRNG
#define MS_PRNG

#include <cstdint>
#include "CircularStack.h"
#include "LinearCongruenceStrategy.h"
#include "LehmerStrategy.h"

namespace MSCrypto{

    template <typename T>
    class MSPrng{
        private:
            MSCrypto::CircularStack<uint32_t, 17> previousNumbers;
            T max;
            T min;
            uint32_t seed;
            T mod;
            MSCrypto::LehmerStrategy calculator;
        public:
            MSPrng();
            MSPrng(T min, T max, uint32_t seed);
            uint32_t get();
            void revert(uint8_t count);
            void reset();
            uint8_t maxResetSize;
            uint8_t getPossibleRevertCount();
    };
};

template <typename T>
MSCrypto::MSPrng<T>::MSPrng(){};

template <typename T>
MSCrypto::MSPrng<T>::MSPrng(T min, T max, uint32_t seed){
    this->min = min;
    this->max = max;
    this->seed = seed;
    this->previousNumbers.push(seed);
    this->calculator = MSCrypto::LehmerStrategy(seed);
    this->maxResetSize = 16;
    this->mod = max - min;
};

template <typename T>
uint8_t MSCrypto::MSPrng<T>::getPossibleRevertCount(){
    return (uint8_t)(this->previousNumbers.size() - 1);
};

template <typename T>
uint32_t MSCrypto::MSPrng<T>::get(){
    uint32_t big_result = calculator.calculate();
    this->previousNumbers.push(big_result);
    return (big_result % mod) + this->min;
};


template <typename T>
void MSCrypto::MSPrng<T>::revert(uint8_t count){
    if( count > 0 && count < this->maxResetSize && this->getPossibleRevertCount() >= count){
        for(int i = 0; i < count; i++){
            this->previousNumbers.pop();
        }
        this->calculator.revert(this->previousNumbers.peek());
    }
};

template <typename T>
void MSCrypto::MSPrng<T>::reset(){
    while(this->previousNumbers.size() > 0){
        this->previousNumbers.pop();
    }
    this->previousNumbers.push(this->seed);
    this->calculator.reset();
};

#endif