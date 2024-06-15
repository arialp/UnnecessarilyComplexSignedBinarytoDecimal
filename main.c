#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <errno.h>
#include <limits.h>
#include <string.h>

int ensureBinary(long long int number){
  // 0..0 is valid
  if(number == 0) return 1;
  
  // input error handling
  if(number < 0) return 0;
  
  int digit = number % 10;
  return (digit == 0 || digit == 1) ? ensureBinary(number / 10) : 0;
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
  return binaryIncrementWrapper(number, 0, 0);
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
  return twosCompWrapper(number, 0, 0);
}

long long int baseConverterWrapper(long long int number, int position){
  if(number == 0) return 0;
  int digit = number % 10;
  long long int result = digit * (1 << position);
  return result + baseConverterWrapper(number / 10, position + 1);
}

long long int baseConverter(char input[]){
  long long int number, tempNumber;
  int sign = 1;
  char *ptr;
  
  input[strcspn(input, "\n")] = '\0';
  
  // Check MSB as a string since int 0x..x is an octal not decimal
  if(input[0] == '1') sign = -1;
  
  number = strtoll(input, &ptr, 10);
  
  // Checks whether strtoll was successful
  if(input == ptr){
    puts("Invalid input (no digits found)!");
    return 0;
  } else if(errno == ERANGE && number == LLONG_MIN){
    puts("Invalid input (underflow)!");
    return 0;
  } else if(errno == ERANGE && number == LLONG_MAX){
    puts("Invalid input (overflow)!");
    return 0;
  } else if(errno != 0 && number == 0){
    puts("Invalid input (unspecified error)");
    return 0;
  }
  if(!ensureBinary(number)){
    puts("Not a valid binary!");
    return 0;
  }
  tempNumber = (sign == -1) ? twosComp(number) : number;
  return sign * baseConverterWrapper(tempNumber, 0);
}

int main(void){
  char input[33];
  puts("-----------Signed Binary to Decimal-----------");
  printf("%s","Enter a binary number (max 16 bits):");
  fgets(input, 33, stdin);
  printf("Decimal: %lld", baseConverter(input));
  return 0;
}
