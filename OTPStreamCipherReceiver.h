#ifndef OTP_STREAM_CIPHER_RECEIVER
#define OTP_STREAM_CIPHER_RECEIVER

#include "Arduino.h"
#ifndef Arduino_h
    #include <cstdint>
    #include <cmath>
    using namespace std;
#endif
#include "MSCrypto.h"
#include "MSPrng.h"
#include "Trivium.h"

namespace MSCrypto{

    template <typename MType, size_t MSize, typename CType, size_t CSize>
    class OTPStreamCipherReceiver{
        private:
            uint32_t seed;
            MSPrng<uint8_t> prng;
            // Trivium csprng;
            uint64_t streamByteLocation;
            uint64_t lastStreamByteLocation;
            uint8_t getRandomByte();
            bool isSecure;
        public:
            OTPStreamCipherReceiver();
            OTPStreamCipherReceiver(uint32_t seed);
            OTPStreamCipherReceiver(uint8_t *key, uint8_t *iv);
            MType parseMessage(uint64_t fullMessage);
            void resetStreamToLastValue();
    };
};

template <typename MType, size_t MSize, typename CType, size_t CSize>
MSCrypto::OTPStreamCipherReceiver<MType, MSize, CType, CSize>::OTPStreamCipherReceiver(){

};

template <typename MType, size_t MSize, typename CType, size_t CSize>
MSCrypto::OTPStreamCipherReceiver<MType, MSize, CType, CSize>::OTPStreamCipherReceiver(uint32_t seed){
    this->seed = seed;
    this->isSecure = false;
    this->prng = MSPrng<uint8_t>(0, 255, seed);
    this->lastStreamByteLocation = 0; 
    this->streamByteLocation = 0;
};

// template <typename MType, size_t Msize, typename CType, size_t CSize>
// MSCrypto::OTPStreamCipherReceiver<MType, MSize, CType, CSize>::OTPStreamCipherReceiver(uint8_t *key, uint8_t *iv){
//     this->isSecure = true;
//     this->csprng = MSCrypto::Trivium(key, iv);
//     this->lastStreamByteLocation = 0;
//     this->streamByteLocation = 0;
// };


template <typename MType, size_t MSize, typename CType, size_t CSize>
uint8_t MSCrypto::OTPStreamCipherReceiver<MType, MSize, CType, CSize>::getRandomByte(){
    this->streamByteLocation++;
    return this->prng.get();
}

template <typename MType, size_t MSize, typename CType, size_t CSize>
MType MSCrypto::OTPStreamCipherReceiver<MType, MSize, CType, CSize>::parseMessage(uint64_t fullMessage){
    CType maxMessageCountNum = (CType)(pow(2, 8*CSize) - 1);
    CType messageCountBytes = fullMessage & maxMessageCountNum;
    MType encodedMessage = fullMessage >> (8 * CSize);
    CType shortStreamByteLocation = (CType)(streamByteLocation % (uint64_t)pow(2, CSize * 8 ));
    CType missedMessages = MSCrypto::rolloverDifference<CType>(messageCountBytes, shortStreamByteLocation);
    uint16_t maxMissedMessages = 10 * MSize;

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
        otpKey += byte * pow(256, i);
    }

    // Decode message
    MType decodedMessage = encodedMessage ^ otpKey;
    return decodedMessage;
}

template <typename MType, size_t MSize, typename CType, size_t CSize>
void MSCrypto::OTPStreamCipherReceiver<MType, MSize, CType, CSize>::resetStreamToLastValue(){
    uint16_t revertCount = this->streamByteLocation - this->lastStreamByteLocation;
    if(!this->prng.revert(revertCount)){
        // NOTE: If lastStreamByteLocation is very high, this could 
        // stall out the arduino. Very dangerous code, may just want 
        // to reset both Xmitter & Receiver.
        this->prng.reset();
        this->streamByteLocation = 0;
        for(uint64_t i = 0; i < this->lastStreamByteLocation; i++){
            this->getRandomByte();
        }
    }else{
        this->streamByteLocation = this->lastStreamByteLocation;
    }
}
#endif