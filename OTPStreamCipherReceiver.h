#ifndef OTP_STREAM_CIPHER_RECEIVER
#define OTP_STREAM_CIPHER_RECEIVER

#include <random>
#include <cmath>
#include <cstdint>
#include "MSCrypto.h"

namespace MSCrypto{

    template <typename MType, size_t MSize, typename CType, size_t CSize>
    class OTPStreamCipherReceiver{
        private:
            std::default_random_engine engine;
            std::uniform_int_distribution<uint8_t> dist;
            MType iv;
            uint8_t getRandomByte();
            uint64_t streamByteLocation;
            uint64_t lastStreamByteLocation;
        public:
            OTPStreamCipherReceiver(MType iv);
            MType parseMessage(uint64_t fullMessage);
            void resetStreamToLastValue();
    };
};

template <typename MType, size_t MSize, typename CType, size_t CSize>
MSCrypto::OTPStreamCipherReceiver<MType, MSize, CType, CSize>::OTPStreamCipherReceiver(MType iv){
    this->iv = iv;
    this->streamByteLocation = 0;
    this->engine.seed(iv);
    this->dist = std::uniform_int_distribution<uint8_t>(0,255);
};


template <typename MType, size_t MSize, typename CType, size_t CSize>
uint8_t MSCrypto::OTPStreamCipherReceiver<MType, MSize, CType, CSize>::getRandomByte(){
    streamByteLocation++;
    return dist(engine);
}

template <typename MType, size_t MSize, typename CType, size_t CSize>
MType MSCrypto::OTPStreamCipherReceiver<MType, MSize, CType, CSize>::parseMessage(uint64_t fullMessage){
    CType maxMessageCountNum = (CType)(std::pow(2, 8*CSize) - 1);
    CType messageCountBytes = fullMessage & maxMessageCountNum;
    MType encodedMessage = fullMessage >> (8 * CSize);
    CType shortStreamByteLocation = (CType)(streamByteLocation % (uint64_t)std::pow(2, CSize * 8 ));
    CType missedMessages = MSCrypto::rolloverDifference<CType>(messageCountBytes, shortStreamByteLocation);
    uint8_t maxMissedMessages = 100 * MSize;

    if(missedMessages > maxMissedMessages || missedMessages % MSize != 0){
        return 0;
    }
    this->lastStreamByteLocation = streamByteLocation;
    // Re-sync steam cipher
    for(CType i = 0; i < missedMessages; i++){
        getRandomByte();
    }

    // Get one-time pad
    MType otpKey = 0;
    for(uint8_t i = 0; i < MSize; i++){
        uint8_t byte = getRandomByte();
        otpKey += byte * std::pow(256, i);
    }

    // Decode message
    MType decodedMessage = encodedMessage ^ otpKey;
    return decodedMessage;
}

template <typename MType, size_t MSize, typename CType, size_t CSize>
void MSCrypto::OTPStreamCipherReceiver<MType, MSize, CType, CSize>::resetStreamToLastValue(){
    this->engine = std::default_random_engine();
    this->engine.seed(this->iv);
    this->dist = std::uniform_int_distribution<uint8_t>(0,255);
    this->streamByteLocation = 0;
    for(uint64_t i=0; i < lastStreamByteLocation; i++){
        getRandomByte();
    }
    this->lastStreamByteLocation = 0;
}
#endif