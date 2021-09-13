#include <iostream>
#include "OTPStreamCipherTransmitterTest.h"

int main(){
    std::cout << "\n\n\n";
    int totalTests = 0;
    int failedTests = 0;

    OTPStreamCipherTransmitterTest otpStreamCipherTest;
    totalTests++;
    if(!otpStreamCipherTest.run()){
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