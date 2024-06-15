#include <stdio.h>
#include <math.h>
#include <stdlib.h>

int ensureBinary(long long int number){
  // 0..0 is valid
  if(number == 0) return 1;
  
  // input error handling
  if(number < 0) return 0;
  
  int digit = number % 10;
  return (digit != 0 && digit != 1) ? 0 : ensureBinary(number / 10);
}

// Quick way to add 1 to a binary
// Scan right to left (LSB to MSB),
// Always negate LSB, then move towards MSB
// Keep negating until a 0 is found, negate once more, done!
long long int binaryIncrementWrapper(long long int number, long long int buffer, int position){
  int digit = number % 10;
  int nextDigit = (number % 100) / 10;
  
  if(!digit) return number + 1;
  if(nextDigit) return binaryIncrementWrapper(number / 10, buffer, position + 1);
  
  // this is only executed when nextDigit is 0
  // so !nextDigit * pow(10, ++position) is the same as this
  buffer += pow(10, ++position);
  buffer += (number / 10) * pow(10, position);
  return buffer;
}

long long int binaryIncrement(long long int number){
  long long int buffer = 0;
  int position = 0;
  return binaryIncrementWrapper(number, buffer, position);
}

long long int twosCompWrapper(long long int number, long long int buffer, int position){
  if(number == 0){
    buffer = binaryIncrement(buffer);
    return buffer;
  }
  
  // negate LSB and add it to buffer
  int lsb = number % 10;
  buffer += !lsb * pow(10, position);
  return twosCompWrapper(number / 10, buffer, position + 1);
}

long long int twosComp(long long int number){
  long long int buffer = 0;
  int position = 0;
  return twosCompWrapper(number, buffer, position);
}

long long int baseConverterWrapper(long long int number, int position){
  if(number == 0) return 0;
  int digit = number % 10;
  long long int result = digit * pow(2, position);
  return result + baseConverterWrapper(number / 10, position + 1);
}

long long int baseConverter(char input[]){
  long long int number, uNumber;
  int sign = 1;
  int length;
  char *ptr;
  
  // Check MSB as a string since int 0x..x is an octal not decimal
  if(input[0] == '1') sign = -1;
  
  // loop to convert char array to long long
  number = strtoll(input, &ptr, 10);
  
  if(!ensureBinary(number)){
    puts("Not a valid binary!");
    return 0;
  }
  if(sign == -1){
    number = twosComp(number);
  }
  
  // drop the sign
  length = (number == 0) ? 1 : log10(number) + 1;
  uNumber = number % (long long)pow(10, length);
  
  return sign * baseConverterWrapper(uNumber, 0);
}

void testBaseConverter(char input[17], long long expected){
  long long result = baseConverter(input);
  printf("Input: %s\nExpected: %lld\nResult: %lld\n\n", input, expected, result);
}

void decimalToBinary(long long n, char *binary, int size){
  for(int i = size - 1; i >= 0; i--){
    binary[i] = (n % 2) + '0';
    n /= 2;
  }
  binary[size] = '\0';
}

int main(void){
  puts("-----------Signed Binary to Decimal Test Cases-----------");
  
  // Loop over every 16-bit binary number
  for(long long i = 0; i < 65536; i++){
    char binary[17];
    decimalToBinary(i, binary, 16);
    long long expected = i < 32768 ? i : i - 65536; // Convert unsigned 16-bit to signed
    printf("Testing %s...\n", binary);
    testBaseConverter(binary, expected);
  }
  
  return 0;
}