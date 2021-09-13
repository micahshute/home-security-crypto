#ifndef OTP_STREAM_CIPHER_TRANSMITTER_TEST
#define OTP_STREAM_CIPHER_TRANSMITTER_TEST

class OTPStreamCipherTransmitterTest{

    public:
        int run();
        int testConstructor();
        int testOtpByte();
        int testGetNumberFromBytes();
        int testOtpMessage();
        int testGetMessageToTransmit();
};

#endif