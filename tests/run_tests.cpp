#include <iostream>
// #include "OTPStreamCipherTransmitterTest.h"
#include "OTPStreamCipherReceiverTest.h"
#include "CircularStackTest.h"
#include "MSPrngTest.h"
#include "ShiftRegisterTest.h"
#include "TriviumTest.h"
#include "CShiftRegisterTest.h"
#include "CTriviumTest.h"

int main(){
    std::cout << "\n\n\n";
    int totalTests = 0;
    int failedTests = 0;

    // OTPStreamCipherTransmitterTest otpStreamCipherTest;
    // totalTests++;

    CShiftRegisterTest cShiftRegisterTest;
    totalTests++;
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
    CTriviumTest cTriviumTest;
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
    if(!cShiftRegisterTest.run()){
        failedTests++;
    }
    if(!triviumTest.run()){
        failedTests++;
    }
    if(!cTriviumTest.run()){
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