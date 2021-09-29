#include <cstdint>
#include <iostream>
#include "TriviumTest.h"
#include "../Trivium.h"
#include <unordered_set>


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

    uint8_t key[80] = {
        1, 1, 0, 1, 0, 0, 1, 0, 1, 0,
        0, 1, 0, 0, 1, 1, 1, 0, 1, 0,
        0, 0, 0, 1, 1, 0, 1, 1, 0, 0,
        0, 1, 1, 1, 0, 1, 0, 1, 1, 1,
        1, 1, 1, 0, 1, 1, 0, 0, 0, 1,
        1, 0, 0, 1, 0, 1, 0, 1, 0, 0,
        0, 1, 0, 1, 1, 0, 1, 0, 1, 1, 
        0, 1, 1, 0, 1, 1, 0, 0, 0, 1
    };

    uint8_t iv[80] = {
        1, 0, 0, 1, 1, 0, 1, 1, 1, 1,
        0, 1, 0, 0, 0, 1, 0, 0, 1, 0,
        1, 1, 1, 0, 1, 0, 0, 1, 0, 1,
        0, 1, 0, 0, 0, 1, 1, 0, 1, 0,
        0, 0, 1, 1, 1, 0, 0, 1, 0, 1,
        0, 0, 1, 1, 1, 0, 0, 1, 0, 0,
        0, 0, 1, 0, 1, 0, 1, 1, 0, 1, 
        1, 1, 0, 0, 1, 0, 1, 1, 0, 1
    };

    MSCrypto::Trivium trivium(key, iv);
    int zeroCount = 0;
    int onesCount = 0;
    int totalIterations = 100000;
    for(int i = 0; i < totalIterations; i++){
        uint8_t bit = trivium.getBit();
        if(bit == 1){ onesCount++; }
        if(bit == 0){ zeroCount++; }
        if(bit != 0 && bit != 1){
            result = 0;
            std::cout << "\tFAILED: Expected bit to be 1 or 0 but it was " << (int)bit << '\n';
            break;
        }
    }
    if(zeroCount + onesCount != totalIterations){
        result = 0;
        std::cout << "\tFAILED: Expected zeroCount + onesCount to be " << totalIterations << " but it was " << (onesCount + zeroCount) << '\n';
    }
    
    // Expect < 0.5% difference in 1s and 0s
    // 0.5% of 100000 = 500
    int diff = zeroCount - onesCount;
    if(diff > 500 || diff < -500){
        result = 0; 
        std::cout << "\tFAILED: Expected zeros and ones to be evenly distributed but there were " << onesCount << " ones and " << zeroCount << " zeroes\n";
    }

    if(result == 0){
        std::cout << "TriviumTest::testGetBit FAILED\n";
    }else{
        std::cout << "TriviumTest::testGetBit PASSED\n";
    }

    return result;
};

int TriviumTest::testGetByte(){
    int result = 1;

    uint8_t key[80] = {
        1, 1, 0, 1, 0, 0, 1, 0, 1, 0,
        0, 1, 0, 0, 1, 1, 1, 0, 1, 0,
        0, 0, 0, 1, 1, 0, 1, 1, 0, 0,
        0, 1, 1, 1, 0, 1, 0, 1, 1, 1,
        1, 1, 1, 0, 1, 1, 0, 0, 0, 1,
        1, 0, 0, 1, 0, 1, 0, 1, 0, 0,
        0, 1, 0, 1, 1, 0, 1, 0, 1, 1, 
        0, 1, 1, 0, 1, 1, 0, 0, 0, 1
    };

    uint8_t iv[80] = {
        1, 0, 0, 1, 1, 0, 1, 1, 1, 1,
        0, 1, 0, 0, 0, 1, 0, 0, 1, 0,
        1, 1, 1, 0, 1, 0, 0, 1, 0, 1,
        0, 1, 0, 0, 0, 1, 1, 0, 1, 0,
        0, 0, 1, 1, 1, 0, 0, 1, 0, 1,
        0, 0, 1, 1, 1, 0, 0, 1, 0, 0,
        0, 0, 1, 0, 1, 0, 1, 1, 0, 1, 
        1, 1, 0, 0, 1, 0, 1, 1, 0, 1
    };

    MSCrypto::Trivium trivium(key, iv);


    int valueCount[256];
    for(int i = 0; i < 256; i++){
        valueCount[i] = 0;
    }

    for(int i = 0; i < 100000; i++){
        uint8_t byte = trivium.getByte();
        valueCount[byte] += 1;
    }

    int max = 0;
    int min = 100000;

    for(int i = 0; i < 256; i++){
        if(valueCount == 0){
            result = 0;
            std::cout << "Expected no number to be 0 but " << i << " was 0\n";
            break;
        }
        if(valueCount[i] > max){
            max = valueCount[i];
        }else if(valueCount[i] < min){
            min = valueCount[i];
        }
    }

    // 100000 numbers evenly divided would be 390 occurances of each
    // number 0-255
    // Hoping the occurance counts are between 342-442, completly arbitrary, naive 
    // expectation
    if(max - min > 100){
        result = 0;
        std::cout << "\tExpected an even distriution of numbers but the max count was " << max << " and the min count was " << min << '\n';
    }

    if(result == 0){
        std::cout << "TriviumTest::testGetByte: FAILED\n";
    }else{
        std::cout << "TriviumTest::testGetByte: PASSED\n";
    }
    return result;
};

int TriviumTest::testPeriodicity(){
    int result = 1;

    char key[81] = "11010010100100111010000010110001110101111110100001100111010001011000110110111001";
    char iv[81] = "10011011110100010010111010100101000111100011100101001110010000101011011100101101";

    MSCrypto::Trivium trivium(key, iv);

    bool initialRepeatFound = false;
    uint64_t count = 0;
    std::unordered_set<uint8_t> set1;
    std::unordered_set<uint8_t> set2;
    std::unordered_set<uint8_t> *activeSet = &set1;

    uint64_t max64 = 18446744073709551615;
    while(count < max64){
        uint32_t num = trivium.getByte();

        if(activeSet->count(num) == 1){
            if(initialRepeatFound){
                break;
            }else{
                initialRepeatFound = true;
                activeSet = &set2;
            }
        }

        activeSet->insert(num);
        count++;
    }
    if(set1.size() == set2.size()){
        result = 0;
        std::cout << "FAILED:  found period before repeats with a size of " << set1.size() << '\n';
    }

    if(set1.size() < 16 || set2.size() < 16){
        result = 0;
        std::cout << "FAILED: Too few numbers before a repeat: " << set1.size() << '\n';
    }

    if( result == 1){
        std::cout << "TriviumTest::testPeriodicity: PASSED\n\n";
    }else{
        std::cout << "TriviumTest::testPeriodicity: FAILED\n\n";
    }

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
        std::cout << "TriviumTest::testRepeatability: FAILED\n";
    }else{
        std::cout << "TriviumTest::testRepeatability: PASSED\n";
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
        std::cout << "TriviumTest::testGetStateAndRevert: FAILED\n";
    }else{
        std::cout << "TriviumTest::testGetStateAndRevert: PASSED\n";
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
        std::cout << "TriviumTest::testReset FAILED\n";
    }else{
        std::cout << "TriviumTest::testReset PASSED\n";
    }
    return result;
};

int TriviumTest::run(){
    int result = 1;
    if(!testSetup()){
        result = 0;
    }
    if(!testGetBit()){
        result = 0;
    }
    if(!testGetByte()){
        result = 0;
    }
    if(!testPeriodicity()){
        result = 0;
    }
    if(!testRepeatability()){
        result = 0;
    }
    if(!testGetStateAndRevert()){
        result = 0;
    }
    if(!testReset()){
        result = 0;
    }
    return result;
};