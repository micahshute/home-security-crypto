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
            StandardRandomStrategy(T seed, T min, T max);
            T getRandomNumber();
            void rollbackStreamLocation();
            void reset();
    };
};

    template <typename T>
    MSCrypto::StandardRandomStrategy<T>::StandardRandomStrategy(T seed, T min, T max) : 
    MSCrypto::RandomStrategy<T>::RandomStrategy(seed, min, max){
        this->engine.seed(seed);
        this->dist = std::uniform_int_distribution<T>(min, max);
    };

    template <typename T>
    T MSCrypto::StandardRandomStrategy<T>::getRandomNumber(){
        this->incrementStreamLocation();
        return dist(engine);
    };

    template <typename T>
    void MSCrypto::StandardRandomStrategy<T>::reset(){
        this->engine = std::default_random_engine();
        this->engine.seed(this->seed);
        this->dist = std::uniform_int_distribution<T>(this->min, this->max);
        this->streamLocation = 0;
        this->lastStreamLocation = 0;
    };

    template <typename T>
    void MSCrypto::StandardRandomStrategy<T>::rollbackStreamLocation(){
        this->engine = std::default_random_engine();
        this->engine.seed(this->seed);
        this->dist = std::uniform_int_distribution<T>(this->min, this->max);
        this->streamLocation = 0;
        for(uint64_t i=0; i < this->lastStreamLocation; i++){
            getRandomNumber();
        }
        this->lastStreamLocation = 0;
    };

#endif