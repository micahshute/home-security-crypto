
#include <cstdint>
#include <iostream>
#include "CShiftRegisterTest.h"
#include "../CShiftRegister.h"


int CShiftRegisterTest::testProperOperation(){
    int result = 1;

    uint8_t aTest[] = {
        0,1,1,0,1,0,0,0,1,1,
        1,1,0,1,0,0,1,1,0,1
    };

    MSCrypto::CShiftRegister<20, 3>asr = MSCrypto::CShiftRegister<20, 3>(aTest);
    for(int i = 0; i < 20; i++){
        if(asr.peek(i) != aTest[i]){
            result = 0;
            std::cout << "\tFAILED: Expected init to work and for SR val " << (int)asr.peek(i) << " to equal test val " << (int)aTest[i] << '\n';
        }
    }

    asr.shift(1);
    if(asr.peek(0) != 1){
        result = 0;
        std::cout << "\tFAILED: Expected shift to first val to work but got " << (int)asr.peek(0) << '\n';
    }

    for(int i = 1; i < 19; i++){
        if(asr.peek(i) != aTest[i-1]){
            result = 0;
            std::cout << "\tFAILED: Expected shift register to work. Wanted " << (int)aTest[i-1] << " but got " << (int)asr.peek(i) << '\n';
        }
    }

    MSCrypto::CShiftRegister<20, 3> shiftr = MSCrypto::CShiftRegister<20, 3>();

    if(shiftr.size() != 20){
        result = 0;
        std::cout << "Expected shift size to be 20 upon instantiation but it was " << shiftr.size() << '\n';
    }

    for(int i = 0; i < 20; i++){
        uint8_t bitToShift = i % 2;
        shiftr.shift(bitToShift);
        if(shiftr.peek(0) != bitToShift){
            result = 0;
            std::cout << "Expected peek value to be " << (int)bitToShift << " but it was " << (int)shiftr.peek(0) << '\n';
            break;
        }
    }

    for(int i = 0; i < 20; i++){
        if(shiftr.peek(i) != ((19 - i) % 2)){
            result = 0;
            std::cout << "Expected peek value at " << i << " to equal " << (i % 2) << " but it was " << (int)shiftr.peek(i) << '\n';
            break;
        }
    }

    for(int i = 0; i < 20; i++){
        uint8_t shiftVal = shiftr.shift(0);
        if(shiftVal != (i % 2)){
            result = 0;
            std::cout << "Expected shift return value to be " <<  (i % 2) << " but it was " << (int)shiftVal << '\n';
            break;
        }
    }
    
    
    int testIndex = 12;
    uint8_t testArr[] = {
        1, 0, 1, 0, 1, 1, 1, 0, 1, 1,
        1, 1, 1, 0, 0, 0, 1, 0, 0, 0,
        1, 0, 1, 1, 0, 0, 1, 0, 1, 0,
        0, 1, 0, 0, 0, 0, 1, 1, 0, 1,
        1, 0, 0, 1, 0, 1, 1, 1, 1, 0,
        0, 1, 1, 1, 0, 0, 1, 1, 0, 1,
        0, 0, 1, 1, 1, 0, 1, 0, 1, 0,
        0, 1, 1, 0, 0, 0, 1, 0, 0, 0,
        1, 0, 1, 1, 0, 1, 1, 0, 1, 1,
        1, 1, 1, 0, 0, 0, 1, 1, 0, 1,
        0, 0, 1, 1, 1, 1, 1, 0, 1, 0,
        1, 1, 1, 0, 0, 0, 1
    };
    for(int i = 0; i < 117; i++){
        uint8_t testVal = shiftr.peek(testIndex);
        shiftr.shift(testArr[i]);
        uint8_t updatedVal = shiftr.peek(testIndex + 1);
        if(testVal != updatedVal){
            std::cout << "Expected values to shift forward by one; expected " << (int)testVal << " to equal " << (int)updatedVal << '\n';
            break;
        }
    }
    

    if(shiftr.peek(0) != testArr[116]){
        result = 0;
        std::cout << "Expected shift register peek to be " << (int)testArr[117 - 20] << " but it was " << (int)shiftr.peek(0) << '\n';
    }

    if(shiftr.peek_back(1) != testArr[117 - 20]){
        result = 0;
        std::cout << "Expected shift register peek_back to be " << (int)testArr[117 - 20] << " but it was " << (int)shiftr.peek_back(1) << '\n';
    }

    for(int i = 0; i < 20; i++){
        uint8_t shiftval = shiftr.shift(0);
        if(shiftval != (testArr[117 - 20 + i])){
            result = 0;
            std::cout << "FAILED: at index: " << i << " Expected shiftval to be " << (int)testArr[117 - 20 + i] << " but it was " << (int)shiftval << '\n';
            break;
        }
    }


    for(int i = 10; i < 30; i++){
        shiftr.shift(i);
    }

    shiftr.setAll(0);
    for(int i = 0; i < 20; i++){
        if(shiftr.peek(i) != 0){
            result = 0;
            std::cout << "Expected all values to be set to 0 but found " << shiftr.peek(i) << " at index " << i << '\n';
            break;
        }
    }

    uint8_t initValues[20] = { 0,0,1,1,0,1,0,0,0,1,0,1,1,1,0,0,0,1,0,1 };
    MSCrypto::CShiftRegister<20, 3>shiftr2(initValues);
    for(int i = 0; i < 20; i++){
        if(shiftr2.peek(i) != initValues[i]){
            result = 0;
            std::cout << "FAILED: Expected shift to match inital array. Should have seen " << (int)initValues[i] << " but saw " << shiftr2.peek(i) << '\n';
            break;
        }
    }

    shiftr2.shift(1);
    for(int i = 0; i < 20; i++){
        bool shouldFail = ((i == 0) && (shiftr2.peek(i) != 1)) || ((i != 0) && (shiftr2.peek(i) != initValues[i-1]));
        if(shouldFail){
            result = 0;
            if(i != 0){
                std::cout << "FAILED: " << i << " Expected shift to match inital array with a shift. Should have seen " << (int)initValues[i-1] << " but saw " << (int)shiftr2.peek(i) << '\n';
            }else{

                std::cout << "FAILED: " << i << " Expected shift to match inital array with a shift. Should have seen " << 1 << " but saw " << (int)shiftr2.peek(i) << '\n';
            }
            break;
        }
    }
    
    
    if(result == 1){
        std::cout << "CShiftRegisterTest#testProperOperation: PASSED\n\n";
    }else{
        std::cout << "CShiftRegisterTest#testProperOperation: FAILED\n\n";
    }
    return result;
}

int CShiftRegisterTest::run(){
    int result = 1;
    if(!testProperOperation()){
        result = 0;
    }
    return result;
};