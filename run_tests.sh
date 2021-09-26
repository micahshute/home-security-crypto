g++ -o test_suite ./tests/run_tests.cpp \
    ./tests/OTPStreamCipherTransmitterTest.cpp \
    ./tests/OTPStreamCipherReceiverTest.cpp \
    ./tests/CircularStackTest.cpp \
    ./tests/MSPrngTest.cpp \
    ./LehmerStrategy.cpp \
    ./MSCrypto.cpp 
./test_suite
rm ./test_suite