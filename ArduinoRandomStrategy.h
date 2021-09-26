
#ifndef ARDUINO_RANDOM_STRATEGY
#define ARDUINO_RANDOM_STRATEGY

#include "RandomStrategy.h"
#include "MSPrng.h"

namespace MSCrypto{
    template <typename T>
    class ArduinoRandomStrategy : public RandomStrategy<T>{
        private:
            MSCrypto::MSPrng<uint8_t> prng;
        public:
            ArduinoRandomStrategy(T seed, T min, T max);
            ArduinoRandomStrategy();
            T getRandomNumber();
            bool rollbackStreamLocation(uint16_t byCount);
            void reset();
    };
};

template <typename T>
MSCrypto::ArduinoRandomStrategy<T>::ArduinoRandomStrategy(){};

template <typename T>
MSCrypto::ArduinoRandomStrategy<T>::ArduinoRandomStrategy(T seed, T min, T max) : 
MSCrypto::RandomStrategy<T>::RandomStrategy(seed, min, max){
    this->prng = MSCrypto::MSPrng<uint8_t>(0,255,seed);
};

template <typename T>
T MSCrypto::ArduinoRandomStrategy<T>::getRandomNumber(){
    return this->prng.get();
};

template <typename T>
void MSCrypto::ArduinoRandomStrategy<T>::reset(){
    this->prng.reset();
};

// TODO: reset if return is false ??
template <typename T>
bool MSCrypto::ArduinoRandomStrategy<T>::rollbackStreamLocation(uint16_t byCount){
    if(this->prng.getPossibleRevertCount() >= byCount){
        this->prng.revert(byCount);
        return true;
    }else{
        return false;
    }
};

#endif