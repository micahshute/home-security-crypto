#ifndef CIRCULAR_STACK
#define CIRCULAR_STACK

#include <cstdint>

namespace MSCrypto{
    template<typename T, std::size_t S>
    class CircularStack{
        private:
            unsigned long index;
            unsigned long length;
            T data[S];
        public:
            CircularStack();
            unsigned long size();
            void push(T item);
            T pop();
            T peek();
            T peek_back(unsigned long prev_index);
            T peek(unsigned long i);
    };
};

template <typename T, std::size_t S>
MSCrypto::CircularStack<T,S>::CircularStack(){
    this->index = S - 1;
    this->length = 0;
};

template <typename T, std::size_t S>
unsigned long MSCrypto::CircularStack<T,S>::size(){
    return this->length;
};

template <typename T, std::size_t S>
void MSCrypto::CircularStack<T,S>::push(T item){
    this->index = (this->index + 1) % S;
    data[this->index] = item;
    if(this->length < S){
        this->length += 1;
    }
};

template <typename T, std::size_t S>
T MSCrypto::CircularStack<T,S>::pop(){
    T result = data[this->index];
    if(this->length > 0){
        this->length -= 1;
        this->index = (this->index - 1 + S) % S;
    }
    return result; // nonsense if empty
};

template <typename T, std::size_t S>
T MSCrypto::CircularStack<T,S>::peek(){
    return data[this->index];
};

template <typename T, std::size_t S>
T MSCrypto::CircularStack<T,S>::peek_back(unsigned long prev_index){
    unsigned long i = (this->index - prev_index + S) % S;
    return data[i];
};

template <typename T, std::size_t S>
T MSCrypto::CircularStack<T,S>::peek(unsigned long i){
    unsigned long index = (this->index + i) % S;
    return data[index];
}

#endif