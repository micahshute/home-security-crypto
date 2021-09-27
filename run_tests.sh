g++ -o test_suite ./tests/run_tests.cpp \
    ./tests/OTPStreamCipherTransmitterTest.cpp \
    ./tests/OTPStreamCipherReceiverTest.cpp \
    ./tests/CircularStackTest.cpp \
    ./tests/MSPrngTest.cpp \
    ./tests/ShiftRegisterTest.cpp \
    ./tests/TriviumTest.cpp \
    ./LehmerStrategy.cpp \
    ./MSCrypto.cpp \
    ./Trivium.cpp
./test_suite
rm ./test_suite