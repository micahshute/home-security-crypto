#ifndef STANDARD_RANDOM_STRATEGY
#define STANDARD_RANDOM_STRATEGY

#include "RandomStrategy.h"
#include <random>

namespace MSCrypto{
    template <typename T>
    class StandardRandomStrategy : public RandomStrategy<T>{
        private:
            std::default_random_engine engine;
            std::uniform_int_distribution<T> dist;
        public:
            StandardRandomStrategy();
            StandardRandomStrategy(uint32_t seed, T min, T max);
            T getRandomNumber();
            bool rollbackStreamLocation(uint16_t byCount);
            void reset();
    };
};

template <typename T>
MSCrypto::StandardRandomStrategy<T>::StandardRandomStrategy(){};

template <typename T>
MSCrypto::StandardRandomStrategy<T>::StandardRandomStrategy(uint32_t seed, T min, T max) : 
MSCrypto::RandomStrategy<T>::RandomStrategy(seed, min, max){
    this->engine.seed(seed);
    this->dist = std::uniform_int_distribution<T>(min, max);
};

template <typename T>
T MSCrypto::StandardRandomStrategy<T>::getRandomNumber(){
    return this->dist(this->engine);
};

template <typename T>
void MSCrypto::StandardRandomStrategy<T>::reset(){
    this->engine = std::default_random_engine();
    this->engine.seed(this->seed);
    this->dist = std::uniform_int_distribution<T>(this->min, this->max);
};

template <typename T>
bool MSCrypto::StandardRandomStrategy<T>::rollbackStreamLocation(uint16_t byCount){
    return false;
};

#endif