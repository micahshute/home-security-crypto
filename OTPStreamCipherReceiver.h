#ifndef OTP_STREAM_CIPHER_RECEIVER
#define OTP_STREAM_CIPHER_RECEIVER

#include "Arduino.h"
#ifndef Arduino_h
    #include <cstdint>
    #include <cmath>
    using namespace std;
#endif
#include "CTrivium.h"
#include "MSCrypto.h"

namespace MSCrypto{

    template <typename MType, size_t MSize, typename CType, size_t CSize>
    class OTPStreamCipherReceiver{
        private:
            CTrivium prng;
            CType streamByteLocation;
            CType lastStreamByteLocation;
            uint8_t getRandomByte();
            bool isSecure;
        public:
            OTPStreamCipherReceiver();
            void setupCipher(uint8_t *key, uint8_t *iv);
            OTPStreamCipherReceiver(uint8_t *key, uint8_t *iv);
            MType parseMessage(uint64_t fullMessage);
            void resetStreamToLastValue();
    };
};

template <typename MType, size_t MSize, typename CType, size_t CSize>
MSCrypto::OTPStreamCipherReceiver<MType, MSize, CType, CSize>::OTPStreamCipherReceiver(){
    Serial.println("MAKING A NEW STREAMCIPHERRECEIVER");
    this->streamByteLocation = 0;
};

template <typename MType, size_t MSize, typename CType, size_t CSize>
MSCrypto::OTPStreamCipherReceiver<MType, MSize, CType, CSize>::OTPStreamCipherReceiver(uint8_t* key, uint8_t* iv){
    this->setupCipher(key, iv);
};

template <typename MType, size_t MSize, typename CType, size_t CSize>
void MSCrypto::OTPStreamCipherReceiver<MType, MSize, CType, CSize>::setupCipher(uint8_t *key, uint8_t *iv){
    this->isSecure = true;
    this->prng = CTrivium(key, iv, true);
    this->streamByteLocation = 0;
    Serial.print("Setting up myself at ref: ");
    Serial.println((unsigned long)this);
    Serial.print("my streambyteloc: ");
    Serial.print(this->streamByteLocation);
    Serial.print(" with ref: ");
    Serial.println((unsigned long) &streamByteLocation);
}

template <typename MType, size_t MSize, typename CType, size_t CSize>
uint8_t MSCrypto::OTPStreamCipherReceiver<MType, MSize, CType, CSize>::getRandomByte(){
    Serial.println("Getting byte...");
    Serial.println(streamByteLocation);
    this->streamByteLocation++;
    return this->prng.get();
}

template <typename MType, size_t MSize, typename CType, size_t CSize>
MType MSCrypto::OTPStreamCipherReceiver<MType, MSize, CType, CSize>::parseMessage(uint64_t fullMessage){
    Serial.println("in ParseMessage OTPSCR");
    Serial.println((uint32_t)fullMessage);
    Serial.print("StreamByteLocation: ");
    Serial.println(streamByteLocation);
    CType maxMessageCountNum = (CType)(pow(2, 8*CSize) - 1);
    CType messageCountBytes = fullMessage & maxMessageCountNum;
    MType encodedMessage = fullMessage >> (8 * CSize);

    CType missedMessages = MSCrypto::rolloverDifference<CType>(messageCountBytes, streamByteLocation);
    uint16_t maxMissedMessages = 15 * MSize;

    Serial.println("ref; Missed ; streamByteLoc; sblref; maxmissed; msgcount");
    Serial.println((unsigned long)this);
    Serial.println((int)missedMessages);
    Serial.println(this->streamByteLocation);
    Serial.println((unsigned long) &streamByteLocation);
    Serial.println((int)maxMissedMessages);
    Serial.println(messageCountBytes);

    if(missedMessages > maxMissedMessages || messageCountBytes % MSize != 0){
        return 0;
    }

    this->prng.cacheState();
    this->lastStreamByteLocation = this->streamByteLocation;

    // Re-sync steam cipher
    for(CType i = 0; i < missedMessages; i++){
        getRandomByte();
    }

    // Get one-time pad
    MType otpKey = 0;
    for(uint8_t i = 0; i < MSize; i++){
        MType byte = (((MType)getRandomByte()) << (8*i));
        otpKey += byte;
    }

    // Decode message
    MType decodedMessage = encodedMessage ^ otpKey;
    return decodedMessage;
}

template <typename MType, size_t MSize, typename CType, size_t CSize>
void MSCrypto::OTPStreamCipherReceiver<MType, MSize, CType, CSize>::resetStreamToLastValue(){
    this->prng.revert();
    this->streamByteLocation = this->lastStreamByteLocation;
    this->lastStreamByteLocation = this->streamByteLocation;
    Serial.print("resetting sbl: ");
    Serial.println(this->streamByteLocation);
}
#endif