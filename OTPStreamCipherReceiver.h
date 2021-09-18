#ifndef OTP_STREAM_CIPHER_RECEIVER
#define OTP_STREAM_CIPHER_RECEIVER

#include <random>
#include <cmath>
#include <cstdint>
#include "Crypto.h"

namespace Crypto{

    template <typename MType, size_t MSize, typename CType, size_t CSize>
    class OTPStreamCipherReceiver{
        private:
            std::default_random_engine engine;
            std::uniform_int_distribution<uint8_t> dist;
            MType iv;
            uint8_t getRandomByte();
            CType streamByteLocation;
        public:
            OTPStreamCipherReceiver(MType iv);
            MType parseMessage(uint64_t fullMessage);
    };
};

template <typename MType, size_t MSize, typename CType, size_t CSize>
Crypto::OTPStreamCipherReceiver<MType, MSize, CType, CSize>::OTPStreamCipherReceiver(MType iv){
    this->iv = iv;
    this->streamByteLocation = 0;
    this->engine.seed(iv);
    this->dist = std::uniform_int_distribution<uint8_t>(0,255);
};


template <typename MType, size_t MSize, typename CType, size_t CSize>
uint8_t Crypto::OTPStreamCipherReceiver<MType, MSize, CType, CSize>::getRandomByte(){
    streamByteLocation++;
    return dist(engine);
}

template <typename MType, size_t MSize, typename CType, size_t CSize>
MType Crypto::OTPStreamCipherReceiver<MType, MSize, CType, CSize>::parseMessage(uint64_t fullMessage){
    CType maxMessageCountNum = (CType)(std::pow(2, 8*CSize) - 1);
    CType messageCountBytes = fullMessage & maxMessageCountNum;
    MType encodedMessage = fullMessage >> (8 * MSize);
    CType missedMessages = Crypto::rolloverDifference<CType>(messageCountBytes, this->streamByteLocation);
    uint8_t maxMissedMessages = 100 * MSize;
    
    if(missedMessages > maxMissedMessages){
        return 0;
    }
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

#endif