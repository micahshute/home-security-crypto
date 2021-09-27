#include <cstdint>
#include <iostream>
#include "ShiftRegisterTest.h"
#include "../ShiftRegister.h"


int ShiftRegisterTest::testProperOperation(){
    int result = 1;
    MSCrypto::ShiftRegister<uint8_t, 20> shiftr = MSCrypto::ShiftRegister<uint8_t, 20>();

    if(shiftr.size() != 20){
        result = 0;
        std::cout << "Expected shift size to be 20 upon instantiation but it was " << shiftr.size() << '\n';
    }

    for(int i = 0; i < 20; i++){
        shiftr.shift(i);
        if(shiftr.peek(0) != i){
            result = 0;
            std::cout << "Expected peek value to be " << i << " but it was " << shiftr.peek(0) << '\n';
            break;
        }
    }

    for(int i = 0; i < 20; i++){
        if(shiftr.peek(i) != 19 - i){
            result = 0;
            std::cout << "Expected peek value at " << i << " to equal " << (19 - i) << " but it was " << (int)shiftr.peek(i) << '\n';
        }
    }

    for(int i = 0; i < 20; i++){
        uint8_t shiftVal = shiftr.shift(0);
        if(shiftVal != i){
            result = 0;
            std::cout << "Expected shift return value to be " <<  i << "but it was " << (int)shiftVal << '\n';
            break;
        }
    }
    
    
    int testIndex = 12;
    for(int i = 0; i < 117; i++){
        uint8_t testVal = shiftr.peek(testIndex);
        shiftr.shift(i);
        uint8_t updatedVal = shiftr.peek(testIndex + 1);
        if(testVal != updatedVal){
            std::cout << "Expected values to shift forward by one; expected " << (int)testVal << " to equal " << (int)updatedVal << '\n';
        }
    }
    

    if(shiftr.peek(0) != 116){
        result = 0;
        std::cout << "Expected shift register peek to be 116 but it was " << (int)shiftr.peek(0) << '\n';
    }

    if(shiftr.peek_back(1) != 116 - 19){
        result = 0;
        std::cout << "Expected shift register peek_back to be 97 but it was " << (int)shiftr.peek_back(1) << '\n';
    }

    for(int i = 0; i < 20; i++){
        uint8_t shiftval = shiftr.shift(0);
        if(shiftval != (116 - 19 + i)){
            result = 0;
            std::cout << "Expected shiftval to be " << (116-20+i) << " but it was " << (int)shiftval << '\n';
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
        }
    }

    uint8_t initValues[20] = { 0,0,1,1,0,1,0,0,0,1,0,1,1,1,0,0,0,1,0,1 };
    MSCrypto::ShiftRegister<uint8_t, 20>shiftr2(initValues);
    for(int i = 0; i < 20; i++){
        if(shiftr2.peek(i) != initValues[i]){
            result = 0;
            std::cout << "FAILED: Expected shift to match inital array. Should have seen " << (int)initValues[i] << " but saw " << shiftr2.peek(i) << '\n';
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
        }
    }
    
    
    if(result == 1){
        std::cout << "ShiftRegisterTest#testProperOperation: PASSED\n\n";
    }else{
        std::cout << "ShiftRegisterTest#testProperOperation: FAILED\n\n";
    }
    return result;
}

int ShiftRegisterTest::run(){
    int result = 1;
    if(!testProperOperation()){
        result = 0;
    }
    return result;
};