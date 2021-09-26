#ifndef RANDOM_STRATEGY
#define RANDOM_STRATEGY

#include <cstdint>

namespace MSCrypto{
    template <typename T>
    class RandomStrategy{
        public:
            uint32_t seed;
            T min;
            T max;
            RandomStrategy();
            RandomStrategy(uint32_t seed, T min, T max);
            T getRandomNumber();
            bool rollbackStreamLocation();
            void reset();

    };
};

template <typename T>
MSCrypto::RandomStrategy<T>::RandomStrategy(){};

template <typename T>
MSCrypto::RandomStrategy<T>::RandomStrategy(uint32_t seed, T min, T max){
    this->seed = seed;
    this->min = min;
    this->max = max;
};

template <typename T>
T MSCrypto::RandomStrategy<T>::getRandomNumber(){};


#endif