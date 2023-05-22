#include <bitset>
#include <iostream>

std::bitset<9> encode(std::bitset<8> data) {
  std::bitset<9> encodedData;
  for (int i = 0; i < 8; ++i) {
    encodedData[i] = data[i];
    if (data[i] == 1) encodedData[8] = !encodedData[8];
  }
  return encodedData;
}

std::bitset<8> decode(std::bitset<9> data) {
  std::bitset<8> decodedData;
  bool parityBit = 0;
  for (int i = 0; i < 8; ++i) {
    decodedData[i] = data[i];
    if (data[i] == 1) parityBit = !parityBit;
  }
  if (parityBit != data[8]) std::cerr << "Данные повреждены!\n";
  return decodedData;
}

int main() {
  std::bitset<8> originalData(0b10101100);
  auto encodedData = encode(originalData);
  std::cout << "Исходные данные:  " << originalData << '\n';
  std::cout << "Кодированные данные: " << encodedData << '\n';
  auto decodedData = decode(encodedData);
  std::cout << "Декодированные данные: " << decodedData << '\n';
  return 0;
}

