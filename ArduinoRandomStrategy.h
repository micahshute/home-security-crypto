
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
            T getRandomNumber();
            bool rollbackStreamLocation();
            void reset();
    };
};

    template <typename T>
    MSCrypto::ArduinoRandomStrategy<T>::ArduinoRandomStrategy(T seed, T min, T max) : 
    MSCrypto::RandomStrategy<T>::RandomStrategy(seed, min, max){
        this->prng = MSCrypto::MSPrng<uint8_t>(0,255,seed);
    };

    template <typename T>
    T MSCrypto::ArduinoRandomStrategy<T>::getRandomNumber(){
        this->incrementStreamLocation();
        return this->prng.get();
    };

    template <typename T>
    void MSCrypto::ArduinoRandomStrategy<T>::reset(){
        this->prng.reset();
        this->streamLocation = 0;
        this->lastStreamLocation = 0;
    };

    // TODO: reset if return is false ??
    template <typename T>
    bool MSCrypto::ArduinoRandomStrategy<T>::rollbackStreamLocation(){
        uint32_t revertCount = this->streamLocation - this->lastStreamLocation;
        if(this->prng.getPossibleRevertCount() >= revertCount){
            this->prng.revert(revertCount);
            this->streamLocation = this->lastStreamLocation;
            return true;
        }else{
            this->prng.reset();
            // If this has been running a long time this could stall out
            // the arduino for a LONG time.... see TODO above
            for(uint64_t i = 0; i < this->lastStreamLocation; i++){
                this->prng.get();
            }
            this->streamLocation = this->lastStreamLocation;
            return true;
        }
    };

#endif