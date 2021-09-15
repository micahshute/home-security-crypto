#include "OTPStreamCipherTransmitter.h"

// template <typename MType, size_t MSize, typename CType, size_t CSize>
// Crypto::OTPStreamCipherTransmitter<MType, MSize, CType, CSize>::OTPStreamCipherTransmitter(MType iv){
//     this->iv = iv;
//     this->streamByteLocation = 0;
//     this->engine.seed(iv);
//     this->dist = std::uniform_int_distribution<uint8_t>(0, 255);
// };

// template <typename MType, size_t MSize, typename CType, size_t CSize>
// uint8_t Crypto::OTPStreamCipherTransmitter<MType, MSize, CType, CSize>::getRandomByte(){
//   streamByteLocation++;
//   return dist(engine);
// };

// template <typename MType, size_t MSize, typename CType, size_t CSize>
// void Crypto::OTPStreamCipherTransmitter<MType, MSize, CType, CSize>::getBytes(MType num, uint8_t byteCount, uint8_t* bytes){
//   Crypto::num2bytes((uint64_t)num, byteCount, bytes);
// };

// template <typename MType, size_t MSize, typename CType, size_t CSize>
// uint8_t Crypto::OTPStreamCipherTransmitter<MType, MSize, CType, CSize>::otpByte(uint8_t byte){
//   return byte ^ getRandomByte();
// };

// template <typename MType, size_t MSize, typename CType, size_t CSize>
// MType Crypto::OTPStreamCipherTransmitter<MType, MSize, CType, CSize>::getNumberFromBytes(uint8_t* bytes){
//   return (MType)Crypto::bytes2num(MSize, bytes);
// }

// template <typename MType, size_t MSize, typename CType, size_t CSize>
// MType Crypto::OTPStreamCipherTransmitter<MType, MSize, CType, CSize>::otpMessage(MType message){
//   uint8_t bytes[MSize];
//   getBytes(message, MSize, bytes);
//   for(int i = 0; i < MSize; i++){
//     bytes[MSize - 1 - i] = otpByte(bytes[MSize - 1 - i]);
//   }
//   return getNumberFromBytes(bytes);
// };

// template <typename MType, size_t MSize, typename CType, size_t CSize>
// MType Crypto::OTPStreamCipherTransmitter<MType, MSize, CType, CSize>::getMessagesSentCount(){
//   return streamByteLocation / MSize;
// }

// template <typename MType, size_t MSize, typename CType, size_t CSize>
// uint64_t Crypto::OTPStreamCipherTransmitter<MType, MSize, CType, CSize>::getMessageToTransmit(MType message){
//   uint8_t bytes[MSize];
//   MType messageNumber = getMessagesSentCount();
//   MType encryptedMessage = otpMessage(message);
//   getBytes(encryptedMessage, MSize, bytes);
//   uint8_t  messageCountBytes[2];
//   getBytes(messageNumber, 2, messageCountBytes);
//   int fullMessage[MSize + CSize];
//   for(int i = 0; i < MSize; i++){
//     fullMessage[i] = bytes[i];
//   }
//   for(int i = 0; i < CSize; i++){
//     fullMessage[i + MSize] = messageCountBytes[i];
//   }
//   return (uint64_t)Crypto::bytes2num(MSize + CSize, fullMessage);
// }
