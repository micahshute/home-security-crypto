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
    // if(!testGetState()){
    //     result = 0;
    // }
    // if(!testRevert()){
    //     result = 0;
    // }
    // if(!testReset()){
    //     result = 0;
    // }
    return result;
};