# Behold! An overcomplicated and super inefficient signed binary to decimal converter!
_Spent 8 hours on this inefficient program just for fun :)_
_Btw, I'm still learning C_

### Why?
No reason at all. Just wanted to see how far I can go until I hit a dead end. Turns out it's possible (partly).
### How?
It converts a signed 16 bit (accuracy limit) binary **in a decimal form** to a decimal value. It never uses the `0b` prefix.

## Functions:
**`ensureBinary()`** ensures that a given integer is correctly in binary form.

**`binaryIncrement()`** increments a binary in decimal form by 1. This is done by scanning from LSB to MSB, and negating until a 0 is found, negating once more and returning. This function runs as a post 1's complement function.

**`twosComp()`** scans a binary in decimal form LSB to MSB, and negates all bits until the number reaches 0. Then calls `binaryIncrement()` to finish.

**`baseConverter()`** takes the input string and checks its sign bit, then converts the string to a long long using `strtoll()`, also checks if it was successful or not. Runs `ensureBinary()` to handle input errors. If sign bit is 1, calls `twosComp()`.

## A few hiccups:
Finding the most significant bit proved to be difficult when working with just integers, it would truncate leading zeros, therefore the MSB would always be 1. That's why the main function first takes the input as a string, finds the MSB, then converts it to an integer.

Whenever there's a leading zero in the input, the compiler would interpret the input as an octal. Another reason to take the input as a string.

Used **one** bit shifting in `baseConverter()` function. _(chatgpt told me it would be more accurate)_

Technically it supports up to 19 bits, but it'll have an error of around +-5 after 16 bits.
