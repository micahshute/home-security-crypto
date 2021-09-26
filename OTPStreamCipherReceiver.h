#ifndef OTP_STREAM_CIPHER_RECEIVER
#define OTP_STREAM_CIPHER_RECEIVER

#include <cmath>
#include <cstdint>
#include "MSCrypto.h"
#include "StandardRandomStrategy.h"

namespace MSCrypto{

    template <typename MType, size_t MSize, typename CType, size_t CSize>
    class OTPStreamCipherReceiver{
        private:
            MType seed;
            StandardRandomStrategy<uint8_t> randomStrategy;
            uint64_t streamByteLocation;
            uint64_t lastStreamByteLocation;
            uint8_t getRandomByte();
        public:
            OTPStreamCipherReceiver(MType seed);
            MType parseMessage(uint64_t fullMessage);
            void resetStreamToLastValue();
    };
};

template <typename MType, size_t MSize, typename CType, size_t CSize>
MSCrypto::OTPStreamCipherReceiver<MType, MSize, CType, CSize>::OTPStreamCipherReceiver(MType seed){
    this->seed = seed;
    this->randomStrategy = StandardRandomStrategy<uint8_t>(seed, 0, 255);
    this->lastStreamByteLocation = 0; 
    this->streamByteLocation = 0;
};


template <typename MType, size_t MSize, typename CType, size_t CSize>
uint8_t MSCrypto::OTPStreamCipherReceiver<MType, MSize, CType, CSize>::getRandomByte(){
    this->streamByteLocation++;
    return this->randomStrategy.getRandomNumber();
}

template <typename MType, size_t MSize, typename CType, size_t CSize>
MType MSCrypto::OTPStreamCipherReceiver<MType, MSize, CType, CSize>::parseMessage(uint64_t fullMessage){
    CType maxMessageCountNum = (CType)(std::pow(2, 8*CSize) - 1);
    CType messageCountBytes = fullMessage & maxMessageCountNum;
    MType encodedMessage = fullMessage >> (8 * CSize);
    CType shortStreamByteLocation = (CType)(streamByteLocation % (uint64_t)std::pow(2, CSize * 8 ));
    CType missedMessages = MSCrypto::rolloverDifference<CType>(messageCountBytes, shortStreamByteLocation);
    uint16_t maxMissedMessages = 100 * MSize;

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
    uint16_t revertCount = this->streamByteLocation - this->lastStreamByteLocation;
    if(!this->randomStrategy.rollbackStreamLocation(revertCount)){
        // NOTE: If lastStreamByteLocation is very high, this could 
        // stall out the arduino. Very dangerous code, may just want 
        // to reset both Xmitter & Receiver.
        this->randomStrategy.reset();
        this->streamByteLocation = 0;
        for(uint64_t i = 0; i < this->lastStreamByteLocation; i++){
            this->getRandomByte();
        }
    }
}
#endif