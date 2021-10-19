#include <iostream>
// #include "OTPStreamCipherTransmitterTest.h"
#include "OTPStreamCipherReceiverTest.h"
#include "CircularStackTest.h"
#include "MSPrngTest.h"
#include "ShiftRegisterTest.h"
#include "TriviumTest.h"

int main(){
    std::cout << "\n\n\n";
    int totalTests = 0;
    int failedTests = 0;

    // OTPStreamCipherTransmitterTest otpStreamCipherTest;
    // totalTests++;
    OTPStreamCipherReceiverTest otpStreamCipherReceiverTest;
    totalTests++;
    CircularStackTest circularStackTest;
    totalTests++;
    MSPrngTest msPrngTest;
    totalTests++;
    ShiftRegisterTest shiftRegisterTest;
    totalTests++;
    TriviumTest triviumTest;
    totalTests++;
    // if(!otpStreamCipherTest.run()){
    //     failedTests++;
    // }
    if(!otpStreamCipherReceiverTest.run()){
        failedTests++;
    }
    if(!circularStackTest.run()){
        failedTests++;
    }
    if(!msPrngTest.run()){
        failedTests++;
    }
    if(!shiftRegisterTest.run()){
        failedTests++;
    }
    if(!triviumTest.run()){
        failedTests++;
    }

    if(failedTests == 0){
        std::cout << "All tests pass: " << totalTests << "/" << totalTests;
    }else{
        std::cout << "Test Suite Failed" << '\n';
        std::cout << "Tests failed: " << failedTests << '\n';
        std::cout << "Tests passed: " << (totalTests - failedTests);
    } 
    std::cout << "\n\n";
    return 1;
}