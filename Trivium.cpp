#include "Trivium.h"

MSCrypto::Trivium::Trivium(uint8_t *key, uint8_t *iv){
    for(int i = 92; i >= 80; i--){
        register1.shift(0);
    }
    for(int i = 0; i < 80; i++){
        this->secretKey[i] = key[i];
        register1.shift(key[80 - 1 - i]);
    }

    for(int i = 83; i >= 80; i--){
        register2.shift(0);
    }
    for(int i = 0; i < 80; i++){
        this->iv[i] = iv[i];
        register2.shift(iv[80 - 1 - i]);
    }

    for(int i = 0; i < 111; i++){
        if(i < 3){
            register3.shift(1);
        }else{
            register3.shift(0);
        }
    }
    
    this->setupTaps();
    this->initialize();

};

MSCrypto::Trivium::Trivium(char *key, char *iv){
    for(int i = 92; i >= 80; i--){
        register1.shift(0);
    }
    for(int i = 0; i < 80; i++){
        char val = key[80 - 1 - i];
        if(val == '1'){
            register1.shift(1);
            this->secretKey[80 - 1 - i] = 1;
        }else{
            register1.shift(0);
            this->secretKey[80 - 1 - i] = 0;
        }
    }

    for(int i = 83; i >= 80; i--){
        register2.shift(0);
    }
    for(int i = 0; i < 80; i++){
        char val = iv[80 - 1 - i];
        if(val == '1'){
            register2.shift(1);
            this->iv[80 - 1 - i] = 1;
        }else{
            register2.shift(0);
            this->iv[80 - 1 - i] = 0;
        }
    }

    for(int i = 0; i < 111; i++){
        if(i < 3){
            register3.shift(1);
        }else{
            register3.shift(0);
        }
    }
    this->setupTaps();
    this->initialize();

};

void MSCrypto::Trivium::setupTaps(){
    RegisterTaps r1;
    RegisterTaps r2;
    RegisterTaps r3;
    r1.output = 65;
    r1.feedback = 68;
    r1.leftAnd = 90;
    r1.rightAnd = 91;
    this->registerTaps1 = r1;

    r2.output = 68;
    r2.feedback = 77;
    r2.leftAnd = 81;
    r2.rightAnd = 82;
    this->registerTaps2 = r2;

    r3.output = 65;
    r3.feedback = 86;
    r3.leftAnd = 108;
    r3.rightAnd = 109;
    this->registerTaps3 = r3;
};

uint8_t MSCrypto::Trivium::getBit(){

    uint8_t r1_out = this->register1.peek(this->registerTaps1.output) ^ this->register1.peek_back(1);
    uint8_t r2_out = this->register2.peek(this->registerTaps2.output) ^ this->register2.peek_back(1);
    uint8_t r3_out = this->register3.peek(this->registerTaps3.output) ^ this->register3.peek_back(1);
    
    uint8_t r1_and = this->register1.peek(this->registerTaps1.leftAnd) & this->register1.peek(this->registerTaps1.rightAnd);
    uint8_t r2_and = this->register2.peek(this->registerTaps2.leftAnd) & this->register2.peek(this->registerTaps2.rightAnd);
    uint8_t r3_and = this->register3.peek(this->registerTaps3.leftAnd) & this->register3.peek(this->registerTaps3.rightAnd);

    uint8_t r1_in = (r3_out ^ r3_and) ^ (this->register1.peek(this->registerTaps1.feedback));
    uint8_t r2_in = (r1_out ^ r1_and) ^ (this->register2.peek(this->registerTaps2.feedback));
    uint8_t r3_in = (r2_out ^ r2_and) ^ (this->register3.peek(this->registerTaps3.feedback));
    
    uint8_t output = r1_out ^ r2_out ^ r3_out;

    this->register1.shift(r1_in);
    this->register2.shift(r2_in);
    this->register3.shift(r3_in);
    
    return output;
};

uint8_t MSCrypto::Trivium::getByte(){
    uint8_t byte = 0;
    uint8_t powers[8] = { 1, 2, 4, 8, 16, 32, 64, 128 };
    for(int i = 0; i < 8; i++){
        byte += getBit() * powers[i];
    }
    return byte;
};

uint8_t MSCrypto::Trivium::calculate(){
    return this->getByte();
};

void MSCrypto::Trivium::initialize(){
    uint16_t initRunCount = 4 * 288;
    for(uint16_t i = 0; i < initRunCount; i++){
        getBit();
    }
};

MSCrypto::Trivium::RegisterState MSCrypto::Trivium::getState(){
    RegisterState state;
    for(uint8_t i = 0; i < 93; i++){
        state.register1[i] = this->register1.peek(i);
    }
    for(uint8_t i = 0; i < 84; i++){
        state.register2[i] = this->register2.peek(i);
    }
    for(uint8_t i = 0; i < 111; i++){
        state.register3[i] = this->register3.peek(i);
    }
    return state;
};

void MSCrypto::Trivium::reset(){
    for(int i = 92; i >= 80; i--){
        register1.shift(0);
    }
    for(int i = 0; i < 80; i++){
        register1.shift(this->secretKey[80 - 1 - i]);
    }

    for(int i = 84; i >= 80; i--){
        register2.shift(0);
    }
    for(int i = 0; i < 80; i++){
        register2.shift(this->iv[80 - 1 - i]);
    }

    for(int i = 0; i < 111; i++){
        if(i < 3){
            register3.shift(1);
        }else{
            register3.shift(0);
        }
    }
    initialize();
};

#include <iostream>
void MSCrypto::Trivium::revert(MSCrypto::Trivium::RegisterState &state){
   for(uint8_t i = 0; i < 93; i++){
       this->register1.shift(state.register1[92 - i]);
   }
   for(uint8_t i = 0; i < 84; i++){
       this->register2.shift(state.register2[83 - i]);
   }
   for(uint8_t i = 0; i < 111; i++){
       this->register3.shift(state.register3[110 - i]);
   }

};