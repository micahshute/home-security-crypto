#include <iostream>
#include <random>
#include <cmath>
#include "OTPStreamCipherTransmitterTest.h"
#include "../OTPStreamCipherTransmitter.h"

#define ULONG_MAX 4294967295

int OTPStreamCipherTransmitterTest::testConstructor(){
    int result = 1;
    unsigned long iv = 123456789;
    Crypto::OTPStreamCipherTransmitter<uint32_t, 4, uint32_t, 4> streamCipher(iv);
    if(streamCipher.streamByteLocation != 0){
        std::cout << "\tFAILED: Expected streamByteLocation to be 0 but it was " << streamCipher.streamByteLocation << '\n';
        result = 0;
    }
    // if(streamCipher.MSize != 4){
    //     std::cout << "\tFAILED: Expected messageByteLength to eqaul 4 but it was " << streamCipher.messageByteLength << '\n'; 
    //     result = 0;
    // }
    if(result == 1){
        std::cout << "OTPStreamCipherTransmitterTest#testConstructor: PASSED\n\n";
    }else{
        std::cout << "OTPStreamCipherTransmitterTest#testConstructor: FAILED\n\n";
    }
    return result;
};

int OTPStreamCipherTransmitterTest::testOtpByte(){
    int result = 1;
    unsigned long iv = 123456789;
    Crypto::OTPStreamCipherTransmitter<uint32_t, 4, uint32_t, 4> streamCipher(iv);
    int encryptedMessages[256];
    for(int i = 0; i < 256; i++){
        int encryptedMsg = streamCipher.otpByte(i);
        encryptedMessages[i] = encryptedMsg;
    } 
    std::default_random_engine engine(iv);
    std::uniform_int_distribution<uint8_t> dist(0,255);

    for(int i = 0; i < 256; i++){
        int otp = dist(engine);
        int decrypted = otp ^ encryptedMessages[i];
        if(decrypted != i){
            result = 0;
            std::cout << "\tFAILED: Expected decrypted otp " << decrypted << " to equal " << i << '\n';
        }
    }
    if(result == 1){
        std::cout << "OTPStreamCipherTransmitterTest#testOtpByte: PASSED\n\n";
    }else{
        std::cout << "OTPStreamCipherTransmitterTest#testOtpByte: FAILED\n\n";
    }
    return result;
}

int OTPStreamCipherTransmitterTest::testGetNumberFromBytes(){
    int result = 1;
    unsigned long iv = 1;
    Crypto::OTPStreamCipherTransmitter<uint32_t, 4, uint32_t, 4>streamCipher(iv);
    uint8_t testNum1[4] = {0, 0, 0, 143};
    uint8_t testNum2[4] = {0, 0, 255, 0};
    uint8_t testNum3[4] = {1, 1, 1, 1};
    uint8_t testNum4[4] = {113, 113, 113, 113};
    uint32_t expectedAnswer1 = 143;
    uint32_t expectedAnswer2 = 65280;
    uint32_t expectedAnswer3 = 16843009;
    uint32_t expectedAnswer4 = 1903260017;

    uint32_t ans1 = streamCipher.getNumberFromBytes(testNum1);
    uint32_t ans2 = streamCipher.getNumberFromBytes(testNum2);
    uint32_t ans3 = streamCipher.getNumberFromBytes(testNum3);
    uint32_t ans4 = streamCipher.getNumberFromBytes(testNum4);

    if(ans1 != expectedAnswer1){
        result = 0;
        std::cout << "\t FAILED: Expected " << ans1 << " to equal " << expectedAnswer1 << '\n';
    }
    if(ans2 != expectedAnswer2){
        result = 0;
        std::cout << "\t FAILED: Expected " << ans2 << " to equal " << expectedAnswer2 << '\n';
    }
    if(ans3 != expectedAnswer3){
        result = 0;
        std::cout << "\t FAILED: Expected " << ans3 << " to equal " << expectedAnswer3 << '\n';
    }
    if(ans4 != expectedAnswer4){
        result = 0;
        std::cout << "\t FAILED: Expected " << ans4 << " to equal " << expectedAnswer4 << '\n';
    }
    
    if(result == 1){
        std::cout << "OTPStreamCipherTransmitterTest#testGetNumberFromBytes: PASSED\n\n";
    }else{
        std::cout << "OTPStreamCipherTransmitterTest#testGetNumberFromBytes: FAILED\n\n";
    }
    return result;
}


int OTPStreamCipherTransmitterTest::testGetMessageToTransmit(){
    int result = 1;

    unsigned long iv = 1214161820;
    uint8_t byteLen = 4;

    Crypto::OTPStreamCipherTransmitter<uint32_t, 4, uint32_t, 4> streamCipher(iv);
    uint32_t message1 = 1903260017;
    uint32_t message2 = 779283;
    uint64_t encodedMessage1 = streamCipher.getMessageToTransmit(message1);
    uint64_t encodedMessage2 = streamCipher.getMessageToTransmit(message2);

    // get last 4 bytes of encodedMessage1
    uint32_t messageCount1 = encodedMessage1 & ULONG_MAX;
    uint32_t messageCount2 = encodedMessage2 & ULONG_MAX;

    uint32_t eMessage1 = encodedMessage1 >> (8*4);
    uint32_t eMessage2 = encodedMessage2 >> (8*4);

    // Message 1

    if(messageCount1 != 0 ){
        result = 0;
        std::cout << "\t FAILED: Expeted " << messageCount1 << " to eq 0\n";
    }

    std::default_random_engine engine(iv);
    std::uniform_int_distribution<uint8_t> dist(0,255);

    uint32_t otpKey = 0;
    // start stream from 0
    for(int i = 0; i < byteLen; i++){
        uint8_t randomByte = dist(engine);
        otpKey += randomByte * std::pow(256, i);
    }

    uint32_t decodedMessage1 = eMessage1 ^ otpKey;

    // message 2
    if(messageCount2 != byteLen){
        result = 0;
        std::cout << "\t FAILED: Expected " << messageCount2 << " to eq 1\n";
    }

    std::default_random_engine engine2(iv);
    std::uniform_int_distribution<int> dist2(0,255);
    uint32_t otpKey2 = 0;
    //prep
    for(int i = 0; i < byteLen; i++){
        dist2(engine2);
    }
    // get correct stream
    for(int i = 0; i < byteLen; i++){
        uint8_t randomByte = dist2(engine2);
        otpKey2 += randomByte * std::pow(256, i);
    }

    uint32_t decodedMessage2 = eMessage2 ^ otpKey2;
    
    if(message1 != decodedMessage1){
        result = 0;
        std::cout << "Expected decoded message " << decodedMessage1 << " to equal original message " << message1 << '\n';
    }
    if(message2 != decodedMessage2){
        result = 0;
        std::cout << "Expected decoded message " << decodedMessage2 << " to equal original message " << message2 << '\n';
    }

    if(result == 1){
        std::cout << "OTPStreamCipherTransmitterTest#testGetMessageToTransmit: PASSED\n\n";
    }else{
        std::cout << "OTPStreamCipherTransmitterTest#testGetMessageToTransmit: FAILED\n\n";
    }
    return result;
}

int OTPStreamCipherTransmitterTest::testOtpMessage(){
    int result = 1;

    uint32_t iv = 2468101214;
    int byteLen = 4;

    Crypto::OTPStreamCipherTransmitter<uint32_t, 4, uint16_t, 2> streamCipher(iv);
    uint32_t message1 = 1903260017;
    uint32_t message2 = 779283;
    uint32_t encodedMessage1 = streamCipher.otpMessage(message1);
    uint32_t encodedMessage2 = streamCipher.otpMessage(message2);

    std::default_random_engine engine(iv);
    std::uniform_int_distribution<uint8_t> dist(0,255);

    uint32_t otpKey = 0;
    for(int i = 0; i < byteLen; i++){
        uint8_t randomByte = dist(engine);
        otpKey += randomByte * std::pow(256, i);
    }

    uint32_t decodedMessage1 = encodedMessage1 ^ otpKey;

    otpKey = 0;
    for(int i = 0; i < byteLen; i++){
        uint8_t randomByte = dist(engine);
        otpKey += randomByte * std::pow(256, i);
    }

    uint32_t decodedMessage2 = encodedMessage2 ^ otpKey;
    
    if(message1 != decodedMessage1){
        result = 0;
        std::cout << "Expected decoded message " << decodedMessage1 << " to equal original message " << message1 << '\n';
    }
    if(message2 != decodedMessage2){
        result = 0;
        std::cout << "Expected decoded message " << decodedMessage2 << " to equal original message " << message2 << '\n';
    }

    if(result == 1){
        std::cout << "OTPStreamCipherTransmitterTest#testOtpMessage: PASSED\n\n";
    }else{
        std::cout << "OTPStreamCipherTransmitterTest#testOtpMessage: FAILED\n\n";
    }
    return result;
}

int OTPStreamCipherTransmitterTest::testStreamCountRollover(){
    int result = 1;

    uint16_t iv = 8192;
    int byteLen = 2;
    Crypto::OTPStreamCipherTransmitter<uint16_t, 2, uint8_t, 1> streamCipher(iv);

    uint16_t message = 24239;
    uint8_t maxCount = 255;

    std::default_random_engine engine(iv);
    std::uniform_int_distribution<uint8_t> dist(0,255);
    
    for(int i = 0; i < 600; i++){
        uint64_t fullMessage = streamCipher.getMessageToTransmit(message);

        uint16_t eMessage = fullMessage >> (16);
        uint8_t msgCount = fullMessage & maxCount;
        if(msgCount != (i % 256)){
            result = 0;
            std::cout << "FAILED: Expected message count " << msgCount << " to equal " << (i % 256) << '\n';
            break;
        }

        uint16_t otpKey = 0;
        for(int j = 0; j < byteLen; j++){
            uint8_t randomByte = dist(engine);
            otpKey += randomByte * std::pow(256, i);
        }

        uint16_t decodedMessage = eMessage ^ otpKey;

        if(decodedMessage != message){
            result = 0;
            std::cout << "FAILED: Expected decoded message " << decodedMessage << " to equal " << message << '\n';
        }

    }

    if(result == 1){
        std::cout << "OTPStreamCipherTransmitterTest#testStreamCountRollover: PASSED\n\n";
    }else{
        std::cout << "OTPStreamCipherTransmitterTest#testStreamCountRollover: FAILED\n\n";
    }

    return result;
}

int OTPStreamCipherTransmitterTest::run(){
    int result = 1;
    if(!testConstructor()){
        result = 0;
    }
    if(!testOtpByte()){
        result = 0;
    }
    if(!testGetNumberFromBytes()){
        result = 0;
    }
    if(!testOtpMessage()){
        result = 0;
    }
    if(!testGetMessageToTransmit()){
        result = 0;
    }
    return result;
};
