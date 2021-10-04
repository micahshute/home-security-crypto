#ifndef OTP_STREAM_CIPHER_TRANSMITTER
#define OTP_STREAM_CIPHER_TRANSMITTER


#include "Arduino.h"
#ifndef Arduino_h
    #include <cmath>
    #include <cstdint>
#endif

#include "MSPrng.h"
#include "MSCrypto.h"

namespace MSCrypto{
    template <typename MType, size_t MSize, typename CType, size_t CSize>
    class OTPStreamCipherTransmitter{
        private: 
            MSPrng<uint8_t> prng;
            uint32_t seed;
            uint8_t getRandomByte();
        public: 
            CType streamByteLocation;
            OTPStreamCipherTransmitter();
            OTPStreamCipherTransmitter(uint32_t seed);
            uint8_t otpByte(uint8_t byte);
            MType otp(MType message);
            void getBytes(MType num, uint8_t byteCount, uint8_t* bytes);
            MType otpMessage(MType message);
            MType getNumberFromBytes(uint8_t* bytes);
            CType getMessagesSentCount();
            CType getByteMessageStartCount();
            uint64_t getMessageToTransmit(MType message); 
    };
};

template <typename MType, size_t MSize, typename CType, size_t CSize>
MSCrypto::OTPStreamCipherTransmitter<MType, MSize, CType, CSize>::OTPStreamCipherTransmitter(){
};

template <typename MType, size_t MSize, typename CType, size_t CSize>
MSCrypto::OTPStreamCipherTransmitter<MType, MSize, CType, CSize>::OTPStreamCipherTransmitter(uint32_t seed){
    this->seed = seed;
    this->streamByteLocation = 0;
    this->prng = MSPrng<uint8_t>(0, 255, seed);
};

template <typename MType, size_t MSize, typename CType, size_t CSize>
uint8_t MSCrypto::OTPStreamCipherTransmitter<MType, MSize, CType, CSize>::getRandomByte(){
    this->streamByteLocation++;
    return this->prng.get();
};

template <typename MType, size_t MSize, typename CType, size_t CSize>
void MSCrypto::OTPStreamCipherTransmitter<MType, MSize, CType, CSize>::getBytes(MType num, uint8_t byteCount, uint8_t* bytes){
    MSCrypto::num2bytes((uint64_t)num, byteCount, bytes);
};

template <typename MType, size_t MSize, typename CType, size_t CSize>
uint8_t MSCrypto::OTPStreamCipherTransmitter<MType, MSize, CType, CSize>::otpByte(uint8_t byte){
    uint8_t rbyte = getRandomByte();
    return byte ^ rbyte;
};

template <typename MType, size_t MSize, typename CType, size_t CSize>
MType MSCrypto::OTPStreamCipherTransmitter<MType, MSize, CType, CSize>::getNumberFromBytes(uint8_t* bytes){
    return (MType)MSCrypto::bytes2num(MSize, bytes);
};

template <typename MType, size_t MSize, typename CType, size_t CSize>
MType MSCrypto::OTPStreamCipherTransmitter<MType, MSize, CType, CSize>::otpMessage(MType message){
    uint8_t bytes[MSize];
    getBytes(message, MSize, bytes);
    for(int i = 0; i < MSize; i++){
        bytes[MSize - 1 - i] = otpByte(bytes[MSize - 1 - i]);
    }
    return getNumberFromBytes(bytes);
};

template <typename MType, size_t MSize, typename CType, size_t CSize>
CType MSCrypto::OTPStreamCipherTransmitter<MType, MSize, CType, CSize>::getMessagesSentCount(){
    return streamByteLocation / MSize;
};

template <typename MType, size_t MSize, typename CType, size_t CSize>
uint64_t MSCrypto::OTPStreamCipherTransmitter<MType, MSize, CType, CSize>::getMessageToTransmit(MType message){
    uint8_t bytes[MSize];
    CType streamByteStart = this->streamByteLocation;
    MType encryptedMessage = otpMessage(message);
    getBytes(encryptedMessage, MSize, bytes);
    uint8_t streamByteStartBytes[CSize];
    MSCrypto::num2bytes((uint64_t)streamByteStart, CSize, streamByteStartBytes);
    uint8_t fullMessage[MSize + CSize];
    for(int i = 0; i < MSize; i++){
        fullMessage[i] = bytes[i];
    }
    for(int i = 0; i < CSize; i++){
        // Serial.print("Stream byte");
        // Serial.println((int)streamByteStartBytes[i]);
        fullMessage[i + MSize] = streamByteStartBytes[i];
    }
    // for(int i = 0; i < (MSize + CSize); i++){
    //     Serial.println((int)fullMessage[i]);
    // }
    // Serial.print("Stream byte location: ");
    // Serial.println(this->streamByteLocation);
    uint32_t test = (unsigned long long)MSCrypto::bytes2num((MSize + CSize), fullMessage);
    // Serial.print("SBL from code: ");
    // Serial.println((test & 255));
    // Serial.println(test);
    return (unsigned long long)MSCrypto::bytes2num((MSize + CSize), fullMessage);
};

#endif