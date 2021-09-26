#ifndef RANDOM_STRATEGY
#define RANDOM_STRATEGY

#include <cstdint>

namespace MSCrypto{
    template <typename T>
    class RandomStrategy{
        private:
            T seed;
            T min;
            T max;
            uint64_t streamLocation;
            uint64_t lastStreamLocation;
            uint64_t incrementStreamLocation();
        public:
            RandomStrategy(T seed, T min, T max);
            uint64_t getStreamLocation();
            T getRandomNumber();
            void rollbackStreamLocation();
            void reset();

    };
};

    template <typename T>
    MSCrypto::RandomStrategy<T>::RandomStrategy(T seed, T min, T max){
        this->seed = seed;
        this->min = min;
        this->max = max;
        this->streamLocation = 0;
        this->lastStreamLocation = 0;
    };

    template <typename T>
    uint64_t MSCrypto::RandomStrategy<T>::getStreamLocation(){
        return this->streamLocation;
    }

    template <typename T>
    uint64_t MSCrypto::RandomStrategy<T>::incrementStreamLocation(){
        this->streamLocation += 1;
        return this->streamLocation;
    }

    template <typename T>
    T MSCrypto::RandomStrategy<T>::getRandomNumber(){};


#endif