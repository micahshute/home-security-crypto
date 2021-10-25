touch ./Arduino.h
g++ -o test_suite ./tests/run_tests.cpp \
    ./tests/OTPStreamCipherReceiverTest.cpp \
    ./tests/CircularStackTest.cpp \
    ./tests/MSPrngTest.cpp \
    ./tests/ShiftRegisterTest.cpp \
    ./tests/TriviumTest.cpp \
    ./tests/CTriviumTest.cpp \
    ./tests/CShiftRegisterTest.cpp \
    ./LehmerStrategy.cpp \
    ./CTrivium.cpp \
    ./MSCrypto.cpp \
    ./Trivium.cpp
./test_suite
rm ./test_suite
rm ./Arduino.h