g++ -o test_suite ./tests/run_tests.cpp \
    ./tests/OTPStreamCipherTransmitterTest.cpp \
    ./tests/OTPStreamCipherReceiverTest.cpp \
    ./Crypto.cpp  
./test_suite
rm ./test_suite