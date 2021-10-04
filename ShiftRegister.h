#ifndef SHIFT_REGISTER
#define SHIFT_REGISTER

#include "Arduino.h"
#ifndef Arduino_h
    #include <cstdint>
    using namespace std;
#endif

namespace MSCrypto{
    template<typename T, size_t S>
    class ShiftRegister{
        private:
            unsigned long startIndex;
            unsigned long length;
            unsigned long getEndIndex();
            void init();
            T data[S];
        public:
            ShiftRegister();
            ShiftRegister(T* initArray);
            unsigned long size();
            T shift(T item);
            T peek();
            T peek_back(unsigned long prev_index);
            T peek(unsigned long i);
            void setAll(T value);
            void set(T *arr);
    };
};

template <typename T, size_t S>
MSCrypto::ShiftRegister<T,S>::ShiftRegister(){
    this->init();
};

template <typename T, size_t S>
MSCrypto::ShiftRegister<T,S>::ShiftRegister(T *initArray){
    this->init();
    this->set(initArray);
}

template <typename T, size_t S>
void MSCrypto::ShiftRegister<T,S>::set(T* arr){
   this->init();
   if(arr != 0){
       for(unsigned long i = 0; i < S; i++){
           this->shift(arr[S - 1 - i]);
       }
   } 
}

template <typename T, size_t S>
void MSCrypto::ShiftRegister<T,S>::init(){
    this->startIndex = 1;
    this->length = S;
}


template <typename T, size_t S>
unsigned long MSCrypto::ShiftRegister<T,S>::getEndIndex(){
    return (this->startIndex + S - 1) % S;
}

template <typename T, size_t S>
unsigned long MSCrypto::ShiftRegister<T,S>::size(){
    return this->length;
};

template <typename T, size_t S>
T MSCrypto::ShiftRegister<T,S>::shift(T item){
    unsigned long endIndex = this->getEndIndex();
    T previousLastValue = data[endIndex];
    this->startIndex = endIndex;
    data[this->startIndex] = item;
    return previousLastValue;
};

template <typename T, size_t S>
T MSCrypto::ShiftRegister<T,S>::peek(unsigned long i){
    unsigned long index = (this->startIndex + i) % S;
    return data[index];
}

template <typename T, size_t S>
T MSCrypto::ShiftRegister<T,S>::peek_back(unsigned long i){
    unsigned long index = (this->startIndex - i + S) % S;
    return data[index];
}

template <typename T, size_t S>
void MSCrypto::ShiftRegister<T,S>::setAll(T value){
    for(unsigned long i = 0; i < S; i++){
        this->shift(value);
    }
}

#endif