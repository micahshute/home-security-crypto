#include <iostream>
#include "OTPStreamCipherReceiverTest.h"
#include "../OTPStreamCipherTransmitter.h"
#include "../OTPStreamCipherReceiver.h"
#include "../StandardRandomStrategy.h"

int OTPStreamCipherReceiverTest::testParseMessage(){
    int result = 1;

    uint16_t iv = 23513;
    uint16_t message = 8072;

    MSCrypto::OTPStreamCipherTransmitter<uint16_t, 2, uint16_t, 2>xmitter(iv);
    MSCrypto::OTPStreamCipherReceiver<uint16_t, 2, uint16_t, 2>receiver(iv);
 
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

    MSCrypto::OTPStreamCipherTransmitter<uint16_t, 2, uint16_t, 2>xmitter2(iv);
    MSCrypto::OTPStreamCipherReceiver<uint16_t, 2, uint16_t, 2>receiver2(iv);

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

    MSCrypto::OTPStreamCipherTransmitter<uint16_t, 2, uint16_t, 2>xmitter3(iv);
    MSCrypto::OTPStreamCipherReceiver<uint16_t, 2, uint16_t, 2>receiver3(iv);

    for(int i = 0; i < 32790; i++){
        xmitter3.getMessageToTransmit(message);
    }
    for(uint16_t i = 0; i < 8; i++){
        uint32_t xmittingMessage = (uint32_t)xmitter3.getMessageToTransmit(message);
        uint16_t decodedMessage = receiver3.parseMessage(xmittingMessage);
        if(decodedMessage == message || decodedMessage == 0){
            result = 0; 
            std::cout << "FAILED incorrect decode check: " << " Expected decoded message " << decodedMessage << " to not equal message " << message << '\n'; 
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

int OTPStreamCipherReceiverTest::testResetStreamToLastValue(){
    int result = 1;

    uint16_t iv = 23513;
    uint16_t message = 8072;

    MSCrypto::OTPStreamCipherTransmitter<uint16_t, 2, uint8_t, 1>xmitter(iv);
    MSCrypto::OTPStreamCipherReceiver<uint16_t, 2, uint8_t, 1>receiver(iv);

    // Run 100 times
    // send a faulty message
    // ensure without reset it fails
    // ensure with reset it does not fail

    for(int i = 0; i < 100; i++){
        uint32_t xmittingMessage = (uint32_t)xmitter.getMessageToTransmit(message);
        uint16_t decodedMessage = receiver.parseMessage(xmittingMessage);
        if(decodedMessage != message){
            result = 0; 
            std::cout << "FAILED missedMessageCheck: " << " Expeccted decoded message " << decodedMessage << " to equal " << message << '\n'; 
            break;
        }
    }
    
    // faked message -> last byte (messageStreamLocation) must 
    // be within 100 of the last sent message. 100 messages sent, so byte count 
    // is at 200. 
    //
    // Let's first make the byte count not divisble by message size so its rejected.
    // So we'll make it 217
    // 10101010 10101010 11011001 = 170 170 217 = 11184857 
    uint32_t failedFakedMessage = 11184857;
    uint16_t failedFakeDecodedMessage = receiver.parseMessage(failedFakedMessage);
    if(failedFakeDecodedMessage != 0){
        result = 0;
        std::cout << "FAILED: Expected fake message to fail because it is not a multiple of MSize" << '\n';
    }

    // OK good, now let's actually trick it
    // 10101010 10101010 11011010 = 170 170 218 = 11184858 
    // First 2 bytes don't matter, the last one determines if it will be "used"
    uint32_t fakedMessage = 11184858;
    uint16_t decodedMessage = receiver.parseMessage(fakedMessage);
    if(decodedMessage == 0){
        result = 0;
        std::cout << "FAILED: Expected faked message to fool receiver but instead got 0" << '\n';
    }

    // Expect the xmitter and receiver to be un-synced now:
    uint32_t failedXmittingMessage = (uint32_t)xmitter.getMessageToTransmit(message);
    uint16_t failedDecodedMessage = receiver.parseMessage(failedXmittingMessage);
    // Expeect ddecodedMessage to be 0
    if(failedDecodedMessage != 0){
        result = 0;
        std::cout << "FAILED: Expected receiver to reject stream location earlier than current location" << '\n';
    }


    // Now, let's do it but reset the receiver when we realized the message was spoofed:

    MSCrypto::OTPStreamCipherTransmitter<uint16_t, 2, uint8_t, 1>xmitter2(iv);
    MSCrypto::OTPStreamCipherReceiver<uint16_t, 2, uint8_t, 1>receiver2(iv);

    // Run 100 times
    // send a faulty message
    // ensure without reset it fails
    // ensure with reset it does not fail

    for(int i = 0; i < 100; i++){
        uint32_t xmittingMessage = (uint32_t)xmitter2.getMessageToTransmit(message);
        uint16_t decodedMessage = receiver2.parseMessage(xmittingMessage);
        if(decodedMessage != message){
            result = 0; 
            std::cout << "FAILED missedMessageCheck2: " << " Expeccted decoded message " << decodedMessage << " to equal " << message << '\n'; 
            break;
        }
    }
    
    // faked message -> last byte (messageStreamLocation) must 
    // be within 100 of the last sent message. Let's make it 218
    // 10101010 10101010 11011010 = 170 170 218 = 11184858 
    // First 2 bytes don't matter, the last one determines if it will be "used"
    uint32_t fakedMessage2 = 11184858;
    uint16_t decodedMessage2 = receiver2.parseMessage(fakedMessage);
    if(decodedMessage2 == 0){
        result = 0;
        std::cout << "FAILED: Expected faked message to fool receiver (2)" << '\n';
    }

    // This time, reset the receiver:
    receiver2.resetStreamToLastValue();

    // Expect the xmitter and receiver to be re-synced now:
    uint32_t correctXmittingMessage = (uint32_t)xmitter2.getMessageToTransmit(message);
    uint16_t correctDecodedMessage = receiver2.parseMessage(failedXmittingMessage);
    // Expeect ddecodedMessage to be 0
    if(correctDecodedMessage != message){
        result = 0;
        std::cout << "FAILED: Expected the receiver and transitter to be re-synced" << '\n';
    }


    if(result == 1){
        std::cout << "OTPStreamCipherReceiverTest#testParseMessage: PASSED\n\n";
    }else{
        std::cout << "OTPStreamCipherReceiverTest#testParseMessage: FAILED\n\n";
    }
    return result;
}


int OTPStreamCipherReceiverTest::run(){
    int result = 1;
    if(!testParseMessage()){
        result = 0;
    }
    if(!testResetStreamToLastValue()){
        result = 0;
    }
    return result;
};