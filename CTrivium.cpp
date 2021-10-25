#include "CTrivium.h"

MSCrypto::CTrivium::CTrivium(){};

MSCrypto::CTrivium::CTrivium(uint8_t *key, uint8_t *iv, bool isCompressed){
    uint8_t reg1[12];
    uint8_t reg2[11];
    uint8_t reg3[14];
    for(uint8_t i = 0; i < 10; i++){
        reg1[i] = key[i];
    }
    reg1[10] = 0;
    reg1[11] = 0;
    for(uint8_t i = 0; i < 10; i++){
        reg2[i] = iv[i];
    }
    reg2[10] = 0;
    for(uint8_t i = 0; i < 13; i++){
        reg3[i] = 0;
    }
    reg3[13] = 14;
    register1.loadCompressed(reg1);
    register2.loadCompressed(reg2);
    register3.loadCompressed(reg3);
    this->setupTaps();
    this->initialize();
}

MSCrypto::CTrivium::CTrivium(uint8_t *key, uint8_t *iv){
    for(uint8_t i = 92; i >= 80; i--){
        register1.shift(0);
    }
    for(uint8_t i = 0; i < 80; i++){
        register1.shift(key[80 - 1 - i]);
    }

    for(uint8_t i = 83; i >= 80; i--){
        register2.shift(0);
    }
    for(uint8_t i = 0; i < 80; i++){
        register2.shift(iv[80 - 1 - i]);
    }

    for(uint8_t i = 0; i < 111; i++){
        if(i < 3){
            register3.shift(1);
        }else{
            register3.shift(0);
        }
    }
    
    this->setupTaps();
    this->initialize();

};

MSCrypto::CTrivium::CTrivium(char *key, char *iv){
    for(int i = 92; i >= 80; i--){
        register1.shift(0);
    }
    for(int i = 0; i < 80; i++){
        char val = key[80 - 1 - i];
        if(val == '1'){
            register1.shift(1);
        }else{
            register1.shift(0);
        }
    }

    for(int i = 83; i >= 80; i--){
        register2.shift(0);
    }
    for(int i = 0; i < 80; i++){
        char val = iv[80 - 1 - i];
        if(val == '1'){
            register2.shift(1);
        }else{
            register2.shift(0);
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

void MSCrypto::CTrivium::cacheState(){
    this->cachedState = this->getState();
    this->hasCachedState = true;
};

void MSCrypto::CTrivium::setupTaps(){
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

uint8_t MSCrypto::CTrivium::getBit(){

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

uint8_t MSCrypto::CTrivium::getByte(){
    uint8_t byte = 0;
    uint8_t powers[8] = { 1, 2, 4, 8, 16, 32, 64, 128 };
    for(int i = 0; i < 8; i++){
        byte += getBit() * powers[i];
    }
    return byte;
};

uint8_t MSCrypto::CTrivium::get(){
    return this->getByte();
};

uint8_t MSCrypto::CTrivium::calculate(){
    return this->getByte();
};

void MSCrypto::CTrivium::initialize(){
    for(int i = 0; i < (4 * 288); i++){
        getBit();
    }
};

MSCrypto::CTrivium::RegisterState MSCrypto::CTrivium::getState(){
    RegisterState state;
    for(uint8_t i = 0; i < 93; i++){
        this->register1.getCompressedState(state.register1);
    }
    for(uint8_t i = 0; i < 84; i++){
        this->register2.getCompressedState(state.register2);
    }
    for(uint8_t i = 0; i < 111; i++){
        this->register3.getCompressedState(state.register3);
    }
    return state;
};

void MSCrypto::CTrivium::revert(MSCrypto::CTrivium::RegisterState &state){
    this->register1.loadCompressed(state.register1);
    this->register2.loadCompressed(state.register2);
    this->register3.loadCompressed(state.register3);
};

bool MSCrypto::CTrivium::revert(){
    if(this->hasCachedState){
        this->revert(this->cachedState);
        this->hasCachedState = false;
        return true;
    }else{
        return false;
    }
};