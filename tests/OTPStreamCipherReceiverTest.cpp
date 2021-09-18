#include <iostream>
#include "OTPStreamCipherReceiverTest.h"
#include "../OTPStreamCipherTransmitter.h"
#include "../OTPStreamCipherReceiver.h"

int OTPStreamCipherReceiverTest::testParseMessage(){
    int result = 1;

    uint16_t iv = 23513;
    uint16_t message = 8072;

    Crypto::OTPStreamCipherTransmitter<uint16_t, 2, uint16_t, 2>xmitter(iv);
    Crypto::OTPStreamCipherReceiver<uint16_t, 2, uint16_t, 2>receiver(iv);
 
    // standard operation

    for(int i = 0; i < 200; i++){
        uint32_t xmittingMessage = (uint32_t)xmitter.getMessageToTransmit(message);
        uint16_t decodedMessage = receiver.parseMessage(xmittingMessage);
        if(decodedMessage != message){
            result = 0;
            std::cout << "FAILED: " << i << " Expected decoded message " << decodedMessage << " to equal sent message " << message << '\n';
            break;
        }
    }

    // when a few messages are missed but not enough to invalidate the message

    // miss 23 messages
    int messagesToMiss = 23;
    for(int i = 0; i < messagesToMiss; i++){
        xmitter.getMessageToTransmit(message);
    }

    for(int i = 0; i < 50; i++){
        uint32_t xmittingMessage = (uint32_t)xmitter.getMessageToTransmit(message);
        uint16_t decodedMessage = receiver.parseMessage(xmittingMessage);
        if(decodedMessage != message){
            result = 0; 
            std::cout << "FAILED after 23 missed messages: " << " Expeccted decoded message " << decodedMessage << " to equal sent message " << message << '\n'; 
            break;
        }
    }

    // miss max-1 more after catching up
    for(int i = 0; i < 99; i++){
        xmitter.getMessageToTransmit(message);
    }
    // Current message count = 372; Current byte count = 744
    // To get to rollover (2**16 bytes), need 32396 more msgs
    // test through the rollover (should get to byte count = 4 aka msg count = 1)
    for(uint16_t i = 0; i < 32397; i++){
        uint32_t xmittingMessage = (uint32_t)xmitter.getMessageToTransmit(message);
        uint16_t decodedMessage = receiver.parseMessage(xmittingMessage);
        if(decodedMessage != message){
            result = 0; 
            std::cout << "FAILED through the rollover: " << " Expeccted decoded message " << decodedMessage << " to equal sent message " << message << '\n'; 
            break;
        }
    }

    // test missed messages through the rollover
    // message count = 1 currently.
    // max message count =  32768
    // Do 32759 messages to get near rollover (8 away from rollover)
    // Miss 20 messages (brings to 12 messages count)
    // Successfully read 8 messages
    // This brings to 20 messages (after rollove)
    // Ensure missed messages over the rollover work correctly
    // 


    for(uint16_t i = 0; i < 32759; i++){
        uint32_t xmittingMessage = (uint32_t)xmitter.getMessageToTransmit(message);
        uint16_t decodedMessage = receiver.parseMessage(xmittingMessage);
        if(decodedMessage != message){
            result = 0; 
            std::cout << "FAILED prepping rollover: " << " Expeccted decoded message " << decodedMessage << " to equal sent message " << message << '\n'; 
            break;
        }
    }
    // miss messages during rollover
    for(int i = 0; i < 20; i++){
        xmitter.getMessageToTransmit(message);
    }
    // can we track it?
    for(uint16_t i = 0; i < 8; i++){
        uint32_t xmittingMessage = (uint32_t)xmitter.getMessageToTransmit(message);
        uint16_t decodedMessage = receiver.parseMessage(xmittingMessage);
        if(decodedMessage != message){
            result = 0; 
            std::cout << "FAILED resyncing after rollover: " << " Expeccted decoded message " << decodedMessage << " to equal sent message " << message << '\n'; 
            break;
        }
    }

    // now we are at 20. 

    // do we correctly lose count over a rollover? 
    // do max messages - 1 missed messages (32767)
    for(uint16_t i = 0; i < 32767; i++){
        xmitter.getMessageToTransmit(message);
    }

    uint32_t xmittingMessage = (uint32_t)xmitter.getMessageToTransmit(message);
    uint16_t decodedMessage = receiver.parseMessage(xmittingMessage);
    if(decodedMessage != 0){
        result = 0; 
        std::cout << "FAILED re-synced when it should not have: " << " Expeccted decoded message " << decodedMessage << " to equal 0" << '\n'; 
    }

    // RESET 
    // Make sure it fails after 101 failed messages
    
    Crypto::OTPStreamCipherTransmitter<uint16_t, 2, uint16_t, 2>xmitter2(iv);
    Crypto::OTPStreamCipherReceiver<uint16_t, 2, uint16_t, 2>receiver2(iv);

    for(int i = 0; i < 101; i++){
        xmitter2.getMessageToTransmit(message);
    }
    
    for(uint16_t i = 0; i < 8; i++){
        uint32_t xmittingMessage = (uint32_t)xmitter2.getMessageToTransmit(message);
        uint16_t decodedMessage = receiver2.parseMessage(xmittingMessage);
        if(decodedMessage != 0){
            result = 0; 
            std::cout << "FAILED missedMessageCheck: " << " Expeccted decoded message " << decodedMessage << " to equal 0" << '\n'; 
            break;
        }
    }

    // RESET
    // Make sure after max+(>= 1 or < 100) messages the message is wrong

    Crypto::OTPStreamCipherTransmitter<uint16_t, 2, uint16_t, 2>xmitter3(iv);
    Crypto::OTPStreamCipherReceiver<uint16_t, 2, uint16_t, 2>receiver3(iv);

    for(int i = 0; i < 32790; i++){
        xmitter3.getMessageToTransmit(message);
    }
    
    for(uint16_t i = 0; i < 8; i++){
        uint32_t xmittingMessage = (uint32_t)xmitter3.getMessageToTransmit(message);
        uint16_t decodedMessage = receiver3.parseMessage(xmittingMessage);
        if(decodedMessage == message || decodedMessage == 0){
            result = 0; 
            std::cout << "FAILED incorrect decode check: " << " Expeccted decoded message " << decodedMessage << " to not equal message " << message << '\n'; 
            break;
        }
    }

    if(result == 1){
        std::cout << "OTPStreamCipherReceiverTest#testParseMessage: PASSED\n\n";
    }else{
        std::cout << "OTPStreamCipherReceiverTest#testParseMessage: FAILED\n\n";
    }
    return result;
};


int OTPStreamCipherReceiverTest::run(){
    int result = 1;
    if(!testParseMessage()){
        result = 0;
    }
    return result;
};