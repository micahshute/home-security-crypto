#ifndef TRIVIUM_TEST
#define TRIVIUM_TEST

class TriviumTest{
    public:
        int testSetup();
        int testGetBit();
        int testGetByte();
        int testPeriodicity();
        int testRepeatability();
        int testGetStateAndRevert();
        int testReset();
        int run();
};

#endif