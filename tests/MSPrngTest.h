
#ifndef MSPRNG_TEST
#define MSPRNG_TEST

class MSPrngTest{
    public:
        int run();
        int testProperOperation();
        int testPeriodicity();
        int testRevert();
        int testReset();
        int testNonzeroMin();
};

#endif 