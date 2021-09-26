#include <cstdint>
#include <iostream>
#include "CircularStackTest.h"
#include "../CircularStack.h"


int CircularStackTest::testProperOperation(){
    int result = 1;
    MSCrypto::CircularStack<uint8_t, 20> stack = MSCrypto::CircularStack<uint8_t, 20>();

    if(stack.size() != 0){
        result = 0;
        std::cout << "Expected stack size to be 0 upon instantiation but it was " << stack.size() << '\n';
    }

    for(int i = 0; i < 10; i++){
        stack.push(i);
        if(stack.peek() != i){
            result = 0;
            std::cout << "Expected peek value to be " << i << " but it was " << stack.peek() << '\n';
            break;
        }
    }

    if(stack.size() != 10){
        result = 0;
        std::cout << "Expected stack size to be 10 but it was " << stack.size() << '\n'; 
    }

    uint8_t popCount = 0;
    while(stack.size() > 0){
        uint8_t popval = stack.pop();
        if(popval != 9 - popCount){
            result = 0;
            std::cout << "Expected pop value to be " << 9-popCount << "but it was " << popval << '\n';
            break;
        }
        popCount += 1;
    }
    
    if(popCount != 10){
        result = 0;
        std::cout << "Expected 10 popped values but there were " << popCount << '\n';
    }
    
    for(int i = 0; i < 117; i++){
        stack.push(i);
        if(i < 20){
            if(stack.size() != i + 1){
                result = 0;
                std::cout << "Expected stack size to be " << i+1 << " but it was " << stack.size() << '\n';
                break;
            }
        }else{
            if(stack.size() != 20){
                result = 0;
                std::cout << "Expected stack size to bve 20 but it was " << stack.size() << '\n';
                break;
            }
        }
    }
    
    if(stack.size() != 20){
        result = 0;
        std::cout << "Expected stack size to bve 20 but it was " << stack.size() << '\n';
    }

    if(stack.peek() != 116){
        result = 0;
        std::cout << "Expected stack peek to be 116 but it was " << (int)stack.peek() << '\n';
    }

    uint8_t popcount2 = 0;
    while(stack.size() > 0){
        uint8_t popval = stack.pop();
        if(popval != 116 - popcount2){
            result = 0;
            std::cout << "Expected popval to be " << 116-popcount2 << " but it was " << popval << '\n';
            break;
        }
        popcount2++;
    }

    if(popcount2 != 20){
        result = 0;
        std::cout << "Expected popcount2 to be 20 but it was " << popcount2 << '\n';
    }

    if(stack.size() != 0){
        result = 0; 
        std::cout << "Expected stack size to be 0 but it was " << stack.size() << '\n';
    }

    stack.pop();
    if(stack.size() != 0){
        result = 0;
        std::cout << "Expected stack size to be 0 after popping an empty stack but it was " << stack.size() << '\n';
    }
    
    if(result == 1){
        std::cout << "CircularStackTest#testProperOperation: PASSED\n\n";
    }else{
        std::cout << "CircularStackTest#testProperOperation: FAILED\n\n";
    }
    return result;
}

int CircularStackTest::run(){
    int result = 1;
    if(!testProperOperation()){
        result = 0;
    }
    return result;
};