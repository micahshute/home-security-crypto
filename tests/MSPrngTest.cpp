#include <cstdint>
#include <iostream>
#include "MSPrngTest.h"
#include "../MSPrng.h"
#include <unordered_set>


int MSPrngTest::testProperOperation(){
    int result = 1;

    uint16_t iv = 117;
    MSCrypto::MSPrng<uint16_t> prng(0,255,iv);

    if(prng.getPossibleRevertCount() != 0){
        result = 0;
        std::cout << "Expected getPossibleRevertCount to be 0 but it was " << prng.getPossibleRevertCount() << '\n';
    }

    if(prng.maxResetSize != 16){
        result = 0;
        std::cout << "Expected maxResetSize to be 16 but it was " << prng.maxResetSize << '\n';
    }

    uint8_t lastNum = 0;
    for(int i = 0; i < 100; i++){
        uint8_t rand = prng.get();
        if(rand < 0 || rand > 255){
            result = 0;
            std::cout << "Expected rand to be between 0 and 255 but it was " << rand << '\n';
            break;
        }
        if(i < prng.maxResetSize){
            if(prng.getPossibleRevertCount() != i + 1){
                result = 0;
                std::cout << "Expected getPossibleRevertCount to be " << i + 1 << " but it was " << prng.getPossibleRevertCount() << '\n';
                break;
            }
        }else{
            if(prng.getPossibleRevertCount() != prng.maxResetSize){
                result = 0;
                std::cout << "Expected getPossibleReertCount to be " << prng.maxResetSize << " but it was " << prng.getPossibleRevertCount();
                break;
            }
        }
        if(lastNum == rand){
            result = 0; 
            std::cout << "Expected not to have consecuitive same numbers but they were both " << rand << '\n';
            break;
        }
        lastNum = rand;
    }
    if(result == 1){
        std::cout << "MSPrngTest#testProperOperation: PASSED\n\n";
    }else{
        std::cout << "MSPrngTest#testProperOperation: FAILED\n\n";
    }

    return result;
};

int MSPrngTest::testPeriodicity(){
    int result = 1;
    // 8-bit 
    // expect not to find periodicity before repeat
    uint32_t iv = 15337191;
    MSCrypto::MSPrng<uint8_t> rand = MSCrypto::MSPrng<uint8_t>(0, 255, iv);
    bool initialRepeatFound = false;
    uint64_t count = 0;
    std::unordered_set<uint8_t> set1;
    std::unordered_set<uint8_t> set2;
    std::unordered_set<uint8_t> *activeSet = &set1;

    uint64_t max64 = 18446744073709551615;
    while(count < max64){
        uint32_t num = rand.get();

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
        std::cout << "FAILED: MSPrngTest#testPeriodicity found period before repeats with a size of " << set1.size() << '\n';
    }
    // if(set1.size() < 16 || set2.size() < 16){
    //     result = 0;
    //     std::cout << "FAILED: Too few numbers before a repeat: " << set1.size() << '\n';
    // }
    if(result == 1){
        std::cout << "MSPrngTest#testPeriodicity: PASSED\n\n";
    }else{
        std::cout << "MSPrngTest#testPeriodicity: FAILED\n\n";
    }
    return result;
};

int MSPrngTest::testRevert(){
    int result = 1;

    uint32_t seed = 123123;
    MSCrypto::MSPrng<uint8_t> rand(0,255,seed);
    
    for(int i = 0; i < 100; i++){
        rand.get();
    }

    uint8_t rnum = rand.get();
    for(int i = 0; i < 4; i++){
        rand.get();
    }

    rand.revert(5);
    uint8_t test_rnum = rand.get();
    if(rnum != test_rnum){
        result = 0;
        std::cout << "Expected revert value: " << (int)test_rnum << " to equal original value: " << rnum << '\n';
    }


    if(result == 1){
        std::cout << "MSPrngTest#testRevert: PASSED\n\n";
    }else{
        std::cout << "MSPrngTest#testRevert: FAILED\n\n";
    }

    return result;
    return result;
};

int MSPrngTest::testReset(){
    int result = 1;

    uint32_t seed = 235321;
    MSCrypto::MSPrng<uint8_t> rand(0,255,seed);
    uint8_t firstVal = rand.get();

    for(int i = 0; i < 12421; i++){
        rand.get();
    }

    rand.reset();
    uint8_t testFirstVal = rand.get();

    if(firstVal != testFirstVal){
        result = 0;
        std::cout << "Expected reset to make " << firstVal << " equal to " << testFirstVal << '\n';
    }

    if(result == 1){
        std::cout << "MSPrngTest#testReset: PASSED\n\n";
    }else{
        std::cout << "MSPrngTest#testReset: FAILED\n\n";
    }
    
    return result;
};

int MSPrngTest::testDistribution(){
    int result = 1;

    uint32_t seed = 353221177;
    MSCrypto::MSPrng<uint8_t> rand(0,255,seed);
    int valueCount[256];
    for(int i = 0; i < 256; i++){
        valueCount[i] = 0;
    }

    for(int i = 0; i < 100000; i++){
        uint8_t byte = rand.get();
        valueCount[byte] += 1;
    }

    int max = 0;
    int min = 100000;
    // Index 255 is always zero, assuming it is 
    // a function of this prng algorithm. Iterate <255 not <256
    for(int i = 0; i < 255; i++){
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

    // 100000 numbers evenly divided would be 392 occurances of each
    // number 0-254
    // Hoping the occurance counts are between 317-467, completly arbitrary, naive 
    // expectation
    if(max - min > 150){
        result = 0;
        std::cout << "\tExpected an even distriution of numbers but the max count was " << max << " and the min count was " << min << '\n';
    }

    return result;
}

int MSPrngTest::run(){
    int result = 1;
    if(!testProperOperation()){
        result = 0;
    }
    if(!testPeriodicity()){
        result = 0;
    }
    if(!testRevert()){
        result = 0;
    }
    if(!testReset()){
        result = 0;
    }
    if(!testDistribution()){
        result = 0;
    }
    return result;
}