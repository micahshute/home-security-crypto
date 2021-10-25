#ifndef CSHIFT_REGISTER
#define CSHIFT_REGISTER

#include "MSCrypto.h"

#include "Arduino.h"
#ifndef Arduino_h
    #include <cstdint>
    using namespace std;
#endif
namespace MSCrypto{
    struct CompressedLocation{
        uint8_t dataIndex;
        uint8_t bitIndex;
    };
    template<size_t S, size_t A>
    class CShiftRegister{
        public:
            CShiftRegister();
            CShiftRegister(uint8_t* initArray);
            unsigned long size();
            uint8_t shift(uint8_t item);
            uint8_t peek();
            uint8_t peek_back(unsigned long prev_index);
            uint8_t peek(unsigned long i);
            void setAll(uint8_t value);
            void set(uint8_t *arr);
            void loadCompressed(uint8_t *arr);
            void getCompressedState(uint8_t* compressedState);
        private:
            unsigned long startIndex;
            unsigned long length;
            unsigned long getEndIndex();
            void init();
            uint8_t get(uint8_t index);
            uint8_t data[A];
            void set(uint8_t index, uint8_t value);
            CompressedLocation getDataLocation(uint8_t index);
    };
};

template <size_t S, size_t A>
MSCrypto::CShiftRegister<S, A>::CShiftRegister(){
    this->init();
};

template <size_t S, size_t A>
MSCrypto::CShiftRegister<S,A>::CShiftRegister(uint8_t *initArray){
    this->init();
    this->set(initArray);
}
template <size_t S, size_t A>
void MSCrypto::CShiftRegister<S,A>::loadCompressed(uint8_t* arr){
    this->startIndex = 0;
    for(uint8_t i = 0; i < A; i++){
        data[i] = arr[i];
    }
}

template <size_t S, size_t A>
void MSCrypto::CShiftRegister<S,A>::getCompressedState(uint8_t* compressedState){
    for(uint8_t i = 0; i < A; i++){
        uint8_t bits[8];
        for(uint8_t j = 0; j < 8; j++){
            bits[j] = (8*i+j) >= S ? 0 : peek((8 * i) + j);
        }
        uint8_t aByte = (uint8_t)MSCrypto::bits2num(8, bits);
        compressedState[i] = aByte; 
    }
}

template <size_t S, size_t A>
void MSCrypto::CShiftRegister<S,A>::set(uint8_t* arr){
   this->init();
   if(arr != 0){
       for(uint8_t i = 0; i < A; i++){
           uint8_t startIndex = i * 8;
           uint8_t endIndex = startIndex + 8;
           if(endIndex > S){
                endIndex = S;
           }
            uint8_t bits[8];
            for(uint16_t j = startIndex; j < (startIndex + 8); j++){
                if(j >= endIndex){
                    bits[j - startIndex] = 0;
                }else{
                    bits[j - startIndex] = arr[j];
                }
            }
            uint8_t aByte = MSCrypto::bits2num(8, bits);
            data[i] = aByte;
       }
   } 
}


template <size_t S, size_t A>
MSCrypto::CompressedLocation MSCrypto::CShiftRegister<S,A>::getDataLocation(uint8_t index){
    uint8_t intDiv = index / 8;
    uint8_t remainder = index % 8;
    CompressedLocation location = {intDiv, remainder};
    return location;
}

template <size_t S, size_t A>
uint8_t MSCrypto::CShiftRegister<S,A>::get(uint8_t index){
    CompressedLocation loc = getDataLocation(index);
    uint8_t theByte = data[loc.dataIndex];
    uint8_t bits[8];
    MSCrypto::num2bits(theByte, 8, bits);
    return bits[loc.bitIndex];
}

template <size_t S, size_t A>
void MSCrypto::CShiftRegister<S,A>::set(uint8_t index, uint8_t value){
    CompressedLocation loc = getDataLocation(index);
    uint8_t theByte = data[loc.dataIndex];
    uint8_t bits[8];
    MSCrypto::num2bits(theByte, 8, bits);
    bits[loc.bitIndex] = value;
    uint8_t newVal = (uint8_t)MSCrypto::bits2num(8, bits);
    data[loc.dataIndex] = newVal;
}


template <size_t S, size_t A>
void MSCrypto::CShiftRegister<S,A>::init(){
    this->startIndex = 0;
    this->length = S;
}


template <size_t S, size_t A>
unsigned long MSCrypto::CShiftRegister<S,A>::getEndIndex(){
    return (this->startIndex + S - 1) % S;
}

template <size_t S, size_t A>
unsigned long MSCrypto::CShiftRegister<S,A>::size(){
    return this->length;
};

template<size_t S, size_t A>
uint8_t MSCrypto::CShiftRegister<S,A>::shift(uint8_t item){
    unsigned long endIndex = this->getEndIndex();
    uint8_t previousLastValue = this->get(endIndex);
    // std::cout << "Previou last value: " << (int)previousLastValue << '\n';
    this->startIndex = endIndex;
    set(startIndex, item);
    return previousLastValue;
};

template<size_t S, size_t A>
uint8_t MSCrypto::CShiftRegister<S,A>::peek(unsigned long i){
    unsigned long index = (this->startIndex + i) % S;
    return this->get(index);
}

template<size_t S, size_t A>
uint8_t MSCrypto::CShiftRegister<S,A>::peek_back(unsigned long i){
    unsigned long index = (this->startIndex - i + S) % S;
    return this->get(index);
}

template<size_t S, size_t A>
void MSCrypto::CShiftRegister<S,A>::setAll(uint8_t value){
    for(unsigned long i = 0; i < S; i++){
        this->shift(value);
    }
}

#endif