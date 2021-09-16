g++ -o test_suite ./tests/run_tests.cpp \
    ./tests/OTPStreamCipherTransmitterTest.cpp \
    ./Crypto.cpp
./test_suite
rm ./test_suite