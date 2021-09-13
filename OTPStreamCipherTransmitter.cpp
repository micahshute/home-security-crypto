#include "OTPStreamCipherTransmitter.h"

OTPStreamCipherTransmitter::OTPStreamCipherTransmitter(int messageByteLength, unsigned long iv){
    this->iv = iv;
    this->streamByteLocation = 0;
    this->messageByteLength = messageByteLength;
    this->engine.seed(iv);
    this->dist = std::uniform_int_distribution<int>(0, 255);
};

int OTPStreamCipherTransmitter::getRandomByte(){
  streamByteLocation++;
  return dist(engine);
}

void OTPStreamCipherTransmitter::getBytes(unsigned long num, int byteCount, int* bytes){
  for(int i = 0; i < byteCount; i++){
    unsigned long bitShift = (byteCount - 1 - i) * 8;
    int byte = num >> bitShift;
    bytes[i] = byte;
    //clear upper bits
    num &= ~(255UL << bitShift);
  }
};

int OTPStreamCipherTransmitter::otpByte(int byte){
  return byte ^ getRandomByte();
};


unsigned long long OTPStreamCipherTransmitter::bytes2long(int byteCount, int* bytes){
  unsigned long long result = 0;
  for(int i = 0; i < byteCount; i++){
    result += bytes[byteCount - 1 - i] * (unsigned long long)std::pow(256, i);
  }
  return result;
}

unsigned long OTPStreamCipherTransmitter::getNumberFromBytes(int* bytes){
  return OTPStreamCipherTransmitter::bytes2long(messageByteLength, bytes);
}

unsigned long OTPStreamCipherTransmitter::otpMessage(unsigned long message){
  int bytes[messageByteLength];
  getBytes(message, messageByteLength, bytes);
  for(int i = 0; i < messageByteLength; i++){
    bytes[messageByteLength - 1 - i] = otpByte(bytes[messageByteLength - 1 - i]);
  }
  return getNumberFromBytes(bytes);
};

unsigned long OTPStreamCipherTransmitter::getMessagesSentCount(){
  return streamByteLocation / messageByteLength;
}

unsigned long long OTPStreamCipherTransmitter::getMessageToTransmit(unsigned long message){
  int bytes[messageByteLength];
  unsigned long messageNumber = getMessagesSentCount();
  unsigned long encryptedMessage = otpMessage(message);
  getBytes(encryptedMessage, messageByteLength, bytes);
  int messageCountBytes[4];
  getBytes(messageNumber, 4, messageCountBytes);
  int fullMessage[messageByteLength + 4];
  for(int i = 0; i < messageByteLength; i++){
    fullMessage[i] = bytes[i];
  }
  for(int i = 0; i < 4; i++){
    fullMessage[i + messageByteLength] = messageCountBytes[i];
  }
  return OTPStreamCipherTransmitter::bytes2long(messageByteLength + 4, fullMessage);
}
