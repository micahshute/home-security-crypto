#include <iostream>
#include "OTPStreamCipherReceiverTest.h"
#include "../OTPStreamCipherTransmitter.h"
#include "../OTPStreamCipherReceiver.h"
// #include "../StandardRandomStrategy.h"
#include "../Trivium.h"

int OTPStreamCipherReceiverTest::testParseMessage(){
    int result = 1;
    uint8_t key[80] = {
        1, 1, 0, 1, 0, 0, 1, 0, 1, 0,
        0, 1, 0, 0, 1, 1, 1, 0, 1, 0,
        0, 0, 0, 0, 1, 0, 1, 1, 0, 0,
        0, 1, 1, 1, 0, 1, 0, 1, 1, 1,
        1, 1, 1, 0, 1, 0, 0, 0, 0, 1,
        1, 0, 0, 1, 1, 1, 0, 1, 0, 0,
        0, 1, 0, 1, 1, 0, 0, 0, 1, 1, 
        0, 1, 1, 0, 1, 1, 1, 0, 0, 1
    };

    uint8_t iv[80] = {
        1, 0, 0, 1, 1, 0, 1, 1, 1, 1,
        0, 1, 0, 0, 0, 1, 0, 0, 1, 0,
        1, 1, 1, 0, 1, 0, 0, 1, 0, 1,
        0, 1, 0, 0, 0, 1, 1, 1, 1, 0,
        0, 0, 1, 1, 1, 0, 0, 1, 0, 1,
        0, 0, 1, 1, 1, 0, 0, 1, 0, 0,
        0, 0, 1, 0, 1, 0, 1, 1, 0, 1, 
        1, 1, 0, 0, 1, 0, 1, 1, 0, 1
    };

    uint16_t message = 11873;

    MSCrypto::OTPStreamCipherTransmitter<uint16_t, 2, uint8_t, 1>xmitter(key, iv);
    MSCrypto::OTPStreamCipherReceiver<uint16_t, 2, uint8_t, 1>receiver(key, iv);
 
    // standard operation

    for(int i = 0; i < 100; i++){
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
    int messagesToMiss = 3;
    for(int i = 0; i < messagesToMiss; i++){
        xmitter.getMessageToTransmit(message);
    }

    for(int i = 0; i < 50; i++){
        uint32_t xmittingMessage = (uint32_t)xmitter.getMessageToTransmit(message);
        uint16_t decodedMessage = receiver.parseMessage(xmittingMessage);
        if(decodedMessage != message){
            result = 0; 
            std::cout << "FAILED after 3 missed messages: " << " Expeccted decoded message " << decodedMessage << " to equal sent message " << message << '\n'; 
            break;
        }
    }

    // miss max-1 more after catching up
    for(int i = 0; i < 14; i++){
        xmitter.getMessageToTransmit(message);
    }
    // Current message count 167
    // Test getting bytes through multiple rollovers
    for(uint16_t i = 0; i < 1000; i++){
        uint32_t xmittingMessage = (uint32_t)xmitter.getMessageToTransmit(message);
        uint16_t decodedMessage = receiver.parseMessage(xmittingMessage);
        if(decodedMessage != message){
            result = 0; 
            std::cout << "FAILED through the rollover: " << " Expeccted decoded message " << decodedMessage << " to equal sent message " << message << '\n'; 
            break;
        }
    }

    // test missed messages through the rollover
    // message count = 1167 
    // Byte count = (1167 * 2) % 256 = 30  currently.
    // max byte count =  255
    // 226 more bytes to get to rollover (@256)
    // get 3 msgs from rollover (6 bytes away -> 250 byte count)
    // Do 110 messages to get to byte count = 250 (3 msgs from rollover)
    // Miss 10 messages (byte count = 14; msgcount = 7)
    // Successfully read 13 messages
    // This brings to 20 messages (after rollove)
    // Ensure missed messages over the rollover work correctly
    // 


    for(uint16_t i = 0; i < 110; i++){
        uint32_t xmittingMessage = (uint32_t)xmitter.getMessageToTransmit(message);
        uint16_t decodedMessage = receiver.parseMessage(xmittingMessage);
        if(decodedMessage != message){
            result = 0; 
            std::cout << "FAILED prepping rollover: " << " Expeccted decoded message " << decodedMessage << " to equal sent message " << message << '\n'; 
            break;
        }
    }
    // miss messages during rollover
    for(int i = 0; i < 10; i++){
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

    //Let's check if count is properly lost over a rollover
    // now we are at 20. 
    // get to 256 - 30 = 226
    // need to do 206 bytes or 103 messages
    for(uint16_t i = 0; i < 103; i++){
        uint32_t xmittingMessage = (uint32_t)xmitter.getMessageToTransmit(message);
        uint16_t decodedMessage = receiver.parseMessage(xmittingMessage);
        if(decodedMessage != message){
            result = 0; 
            std::cout << "FAILED prepping failed rollover: " << " Expeccted decoded message " << decodedMessage << " to equal sent message " << message << '\n'; 
            break;
        }
    }

    // do we correctly lose count over a rollover? 
    // miss 16 messages over a rollover
    for(uint16_t i = 0; i < 16; i++){
        xmitter.getMessageToTransmit(message);
    }

    uint32_t xmittingMessage = (uint32_t)xmitter.getMessageToTransmit(message);
    uint16_t decodedMessage = receiver.parseMessage(xmittingMessage);
    if(decodedMessage != 0){
        result = 0; 
        std::cout << "FAILED re-synced when it should not have: " << " Expeccted decoded message " << decodedMessage << " to equal 0" << '\n'; 
    }

    // RESET 
    // Make sure it fails after 16 failed messages

    MSCrypto::OTPStreamCipherTransmitter<uint16_t, 2, uint8_t, 1>xmitter2(key, iv);
    MSCrypto::OTPStreamCipherReceiver<uint16_t, 2, uint8_t, 1>receiver2(key, iv);

    for(int i = 0; i < 16; i++){
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

    MSCrypto::OTPStreamCipherTransmitter<uint16_t, 2, uint8_t, 1>xmitter3(key, iv);
    MSCrypto::OTPStreamCipherReceiver<uint16_t, 2, uint8_t, 1>receiver3(key, iv);

    for(int i = 0; i < 130; i++){
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

    uint8_t key[80] = {
        1, 1, 0, 1, 0, 0, 1, 0, 1, 0,
        0, 1, 0, 0, 1, 1, 1, 0, 1, 0,
        0, 0, 0, 0, 1, 0, 1, 1, 0, 0,
        0, 1, 1, 1, 0, 1, 0, 1, 1, 1,
        1, 1, 1, 0, 1, 0, 0, 0, 0, 1,
        1, 0, 0, 1, 1, 1, 0, 1, 0, 0,
        0, 1, 0, 1, 1, 0, 0, 0, 1, 1, 
        0, 1, 1, 0, 1, 1, 1, 0, 0, 1
    };

    uint8_t iv[80] = {
        1, 0, 0, 1, 1, 0, 1, 1, 1, 1,
        0, 1, 0, 0, 0, 1, 0, 0, 1, 0,
        1, 1, 1, 0, 1, 0, 0, 1, 0, 1,
        0, 1, 0, 0, 0, 1, 1, 1, 1, 0,
        0, 0, 1, 1, 1, 0, 0, 1, 0, 1,
        0, 0, 1, 1, 1, 0, 0, 1, 0, 0,
        0, 0, 1, 0, 1, 0, 1, 1, 0, 1, 
        1, 1, 0, 0, 1, 0, 1, 1, 0, 1
    };

    uint16_t message = 8072;

    MSCrypto::OTPStreamCipherTransmitter<uint16_t, 2, uint8_t, 1>xmitter(key, iv);
    MSCrypto::OTPStreamCipherReceiver<uint16_t, 2, uint8_t, 1>receiver(key, iv);

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

    MSCrypto::OTPStreamCipherTransmitter<uint16_t, 2, uint8_t, 1>xmitter2(key, iv);
    MSCrypto::OTPStreamCipherReceiver<uint16_t, 2, uint8_t, 1>receiver2(key, iv);

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
    // be within 30 of the last sent message. Let's make it 218
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
    uint16_t correctDecodedMessage = receiver2.parseMessage(correctXmittingMessage);
    // Expeect ddecodedMessage to be 0
    if(correctDecodedMessage != message){
        result = 0;
        std::cout << "FAILED: Expected the receiver and transitter to be re-synced" << '\n';
    }

    // TEST when number is added to ID
    for(int i = 0; i < 500; i++){
        uint32_t xmitCode = xmitter2.getMessageToTransmit(message);
        xmitCode = xmitCode << 8; // add ID
        uint16_t decodedMessage =  receiver2.parseMessage(xmitCode >> 8);
        if(decodedMessage != message){
            result = 0;
            std::cout << "FAILED: with added id, expected decoded message to be " << message << " but it was " << decodedMessage << '\n';
            break;
        }
    }


    // Works with multiple resends

    for(int i = 0; i < 1000; i++){
        uint32_t xmitCode = xmitter2.getMessageToTransmit(message);
        uint16_t decodedMessage = receiver2.parseMessage(xmitCode);
        if(decodedMessage != message){
            result = 0;
            std::cout << "\tFAILED: expected decoded message " << decodedMessage << " to equal message " << message << '\n'; 
            break;
        }
        for(int i = 0; i < 9; i++){
            uint16_t decodedRepeatMessage = receiver2.parseMessage(xmitCode);
            if(decodedRepeatMessage != 0){
                result = 0;
                std::cout << "\tFAILED: expected decoded message " << decodedMessage << " to equal 0\n";
                break;
            }
        }
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