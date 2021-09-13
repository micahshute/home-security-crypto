#ifndef OTP_STREAM_CIPHER_TRANSMITTER
#define OTP_STREAM_CIPHER_TRANSMITTER

#include <random>
#include <cmath>

class OTPStreamCipherTransmitter{
    private: 
        std::default_random_engine engine;
        std::uniform_int_distribution<int> dist;
        unsigned long iv;
        int getRandomByte();
    public: 
        static unsigned long long bytes2long(int byteCount, int* bytes);
        int messageByteLength;
        unsigned long streamByteLocation;
        OTPStreamCipherTransmitter(int messageByteLength, unsigned long iv);
        int otpByte(int byte);
        unsigned long otp(unsigned long message);
        void getBytes(unsigned long num, int byteCount, int* bytes);
        unsigned long otpMessage(unsigned long message);
        unsigned long getNumberFromBytes(int* bytes);
        unsigned long getMessagesSentCount();
        unsigned long long getMessageToTransmit(unsigned long message); 
};


#endif