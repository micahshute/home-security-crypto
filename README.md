# Trivium for Arduino


This library is unpolished, but should allow you to send and receive encrypted messages between arduino devices using the Trivium stream cipher. CTrivium is "compressed" to save space on the arduino, ie not storing a single bit as a `uint8` but instead storing 8 bits, which makes the abstraction of a shift register a little harder to follow logically but is nice if you want to have a few instances of trivium running on a single arduino without running out of memory space.

Looking at the tests is probably the best way to get familiar, the easiest way to send messages in your program is to do something like this:

```c
    uint8_t ckey[10] = {
        210, 147, 161, 177, 215, 236, 101, 69, 173, 177
    };
    // NOTE: In my projects I use the Entropy library to randomly generate the IV and send it over
    // in cleartext to the arduino running the receiver.
    uint8_t civ[10] = {
        155, 209, 46, 149, 26, 57, 78, 66, 183, 45
    };

    uint16_t message = 11873;

    // The generics here are <encodedMessageType, encodedMessageLengthInBytes, cleartextMessagePortionType, cleartextMessagePortionLengthInBytes>
    MSCrypto::OTPStreamCipherTransmitter<uint16_t, 2, uint8_t, 1>xmitter(ckey, civ);
    uint32_t xmittingMessage = (uint32_t)xmitter.getMessageToTransmit(message);
    // Then send this over to your RF transmitter
```

And to receive them:

```c
// Same generics as above
MSCrypto::OTPStreamCipherReceiver<uint16_t, 2, uint8_t, 1>receiver;
// get the ckey and civ in cleartext from the transmitter
receiver.setupCipher(ckey, civ);
// get receivedMessage from the transmitter 
uint16_t decodedMessage = receiver.parseMessage(receivedMessage);
```

Hopefully I'll later get rid of unused files in this repo and document better, but it should at least be usable now. 
The deault for OTPStreamCipherReceiver/Transmitter is the "compressed" version of things, eg `CTrivium`, `CShiftRegister`. This is a little slower than `Trivium` and `ShiftRegister`, but should save a lot of space which is usually the limiting factor.