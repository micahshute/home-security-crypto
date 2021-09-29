#include <cstdint>
#include <iostream>
#include "TriviumTest.h"
#include "../Trivium.h"


int TriviumTest::testSetup(){
    int result = 1;
    // Setup with bit array
    uint8_t key[80] = {
        1, 1, 0, 1, 0, 0, 1, 0, 1, 0,
        0, 1, 0, 0, 1, 1, 1, 0, 1, 0,
        0, 0, 0, 0, 1, 0, 1, 1, 0, 0,
        0, 1, 1, 1, 0, 1, 0, 1, 1, 1,
        1, 1, 1, 0, 1, 0, 0, 0, 0, 1,
        1, 0, 0, 1, 1, 1, 0, 1, 0, 0,
        0, 1, 0, 1, 1, 0, 0, 0, 1, 1, 
        0, 1, 1, 0, 1, 1, 1, 0, 0, 1
    };

    uint8_t iv[80] = {
        1, 0, 0, 1, 1, 0, 1, 1, 1, 1,
        0, 1, 0, 0, 0, 1, 0, 0, 1, 0,
        1, 1, 1, 0, 1, 0, 0, 1, 0, 1,
        0, 1, 0, 0, 0, 1, 1, 1, 1, 0,
        0, 0, 1, 1, 1, 0, 0, 1, 0, 1,
        0, 0, 1, 1, 1, 0, 0, 1, 0, 0,
        0, 0, 1, 0, 1, 0, 1, 1, 0, 1, 
        1, 1, 0, 0, 1, 0, 1, 1, 0, 1
    };

    MSCrypto::Trivium trivium(key, iv);
    std::cout << "Successfully initialized Trivium with bit array\n";
    uint8_t firstByteBitInit = trivium.getByte();
    // Setup with char array

    char key2[81] = "11010010100100111010000010110001110101111110100001100111010001011000110110111001";
    char iv2[81] = "10011011110100010010111010010101000111100011100101001110010000101011011100101101";
    MSCrypto::Trivium trivium2(key2, iv2);
    std::cout << "Successfully initialized Trivium with char array\n";

    uint8_t firstByteCharInit = trivium2.getByte();


    if(firstByteBitInit != firstByteCharInit){
        result = 0;
        std::cout << "Expected the two trivium instances to initialize identically ";
        std::cout << "but the bit init got " << (int)firstByteBitInit << " and the char init got " << (int)firstByteCharInit << '\n';
    }



    // TODO: Setup with byte array

    if(result == 0){
        std::cout << "TriviumTest::testSetup: FAILED\n";
    }else{
        std::cout << "TiviumTest::testSetup: PASSED\n";
    }
    return result;
}

int TriviumTest::testGetBit(){
    int result = 1;

    return result;
};

int TriviumTest::testGetByte(){
    int result = 1;

    return result;
};

int TriviumTest::testPeriodicity(){
    int result = 1;

    return result;
};

int TriviumTest::testRepeatability(){
    int result = 1;

    char key[81] = "11010010100100111010000010110001110101111110100001100111010001011000110110111001";
    char iv[81] = "10011011110100010010111010100101000111100011100101001110010000101011011100101101";
    MSCrypto::Trivium trivium(key, iv);
    MSCrypto::Trivium trivium2(key, iv);
    MSCrypto::Trivium trivium3(key, iv);

    for(uint32_t i = 0; i < 1000000; i++){
        uint8_t byte1 = trivium.getByte();
        uint8_t byte2 = trivium2.getByte();
        uint8_t byte3 = trivium3.getByte();
        if(byte1 != byte2 || byte1 != byte3){
            result = 0;
            std::cout << "\tFAILED: Expected trivium to give repeatable results, but got from 3 separate instances: " << byte1 << ", " << byte2 << ", " << byte3 << '\n';
            break;
        }
    }

    if(result == 0){
        std::cout << "TriviumTest Repeatability Test: FAILED\n";
    }else{
        std::cout << "TriviumTest Repeatability Test: PASSED\n";
    }
    return result;
};

int TriviumTest::testGetStateAndRevert(){
    int result = 1;

    uint8_t key[80] = {
        1, 1, 0, 1, 0, 0, 1, 0, 1, 0,
        0, 1, 0, 0, 1, 1, 1, 0, 1, 0,
        0, 0, 0, 0, 1, 0, 1, 1, 0, 0,
        0, 1, 1, 1, 0, 1, 0, 1, 1, 1,
        1, 1, 1, 0, 1, 0, 0, 0, 0, 1,
        1, 0, 0, 1, 1, 1, 0, 1, 0, 0,
        0, 1, 0, 1, 1, 0, 0, 0, 1, 1, 
        0, 1, 1, 0, 1, 1, 1, 0, 0, 1
    };

    uint8_t iv[80] = {
        1, 0, 0, 1, 1, 0, 1, 1, 1, 1,
        0, 1, 0, 0, 0, 1, 0, 0, 1, 0,
        1, 1, 1, 0, 1, 0, 0, 1, 0, 1,
        0, 1, 0, 0, 0, 1, 1, 1, 1, 0,
        0, 0, 1, 1, 1, 0, 0, 1, 0, 1,
        0, 0, 1, 1, 1, 0, 0, 1, 0, 0,
        0, 0, 1, 0, 1, 0, 1, 1, 0, 1, 
        1, 1, 0, 0, 1, 0, 1, 1, 0, 1
    };

    MSCrypto::Trivium trivium(key, iv);
    for(int i = 0; i < 1021; i++){
        trivium.getByte();
    }

    MSCrypto::Trivium::RegisterState savedState = trivium.getState();

    MSCrypto::Trivium newTrivium(key, iv);
    newTrivium.revert(savedState);

    for(int i = 0; i < 100; i++){
        uint8_t origByte = trivium.getByte();
        uint8_t revertedByte = newTrivium.getByte();
        if(origByte != revertedByte){
            result = 0;
            std::cout << "FAILED: "<< i << " Expected reverted trivium to match the original but got original ; reverted " << (int)origByte << " ; " << (int)revertedByte << '\n';
            break;
        }
    }

    trivium.revert(savedState);
    MSCrypto::Trivium newest(key, iv);

    for(int i = 0; i < 1021; i++){
        newest.getByte();
    }

    
    for(int i = 0; i < 100; i++){
        uint8_t origByte = trivium.getByte();
        uint8_t revertedByte = newest.getByte();
        if(origByte != revertedByte){
            result = 0;
            std::cout << "FAILED: (revert 2) " << i << " Expected reverted trivium to match the original but got original ; reverted " << (int)origByte << " ; " << (int)revertedByte << '\n';
            break;
        }
    }

    if(result == 0){
        std::cout << "TriviumTest::getState and TriviumTest::getRevert: FAILED\n";
    }else{
        std::cout << "TriviumTest::getState and TriviumTest::revert: PASSED\n";
    }

    return result;
};

int TriviumTest::testReset(){
    int result = 1;

    uint8_t firstNums[101];
    char key[81] = "11010010100100111010000010110001110101111110100001100111010001011000110110111001";
    char iv[81] = "10011011110100010010111010100101000111100011100101001110010000101011011100101101";
    MSCrypto::Trivium trivium(key, iv);
    for(int i = 0; i < 101; i++){
        firstNums[i] = trivium.getByte();
    }

    trivium.reset();
    for(int i = 0; i < 101; i++){
        uint8_t newByte = trivium.getByte();
        if(newByte != firstNums[i]){
            result = 0;
            std::cout << "\tFAILED: Expected reset to work, but got original ; newnum" << (int)firstNums[i] << " ; " << (int)newByte << '\n';
            break;
        }
    }

    if(result == 0){
        std::cout << "TriviumTest::reset FAILED\n";
    }else{
        std::cout << "TriviumTest::reset PASSED\n";
    }
    return result;
};

int TriviumTest::run(){
    int result = 1;
    if(!testSetup()){
        result = 0;
    }
    // if(!testGetBit()){
    //     result = 0;
    // }
    // if(!testGetByte()){
    //     result = 0;
    // }
    // if(!testPeriodicity()){
    //     result = 0;
    // }
    if(!testRepeatability()){
        result = 0;
    }
    if(!testGetStateAndRevert()){
        result = 0;
    }
    // if(!testReset()){
    //     result = 0;
    // }
    return result;
};