# password-generator
Generates a strong 25 length password from ASCII. (BUILT FOR *NIX and MacOS SYSTEMS!)

## Building
To build the base program:
```gcc main.c -o pw-gen```
I also included a quick program made by ChatGPT to calculate the average of my bit entropy `dataset`, if you want to verify the average you can
compile the `average_calculator.c` program.

## About
Pretty much, the program generates a 25-byte long string from an matrix of numbers that represent ASCII characters.
The average bit entropy of the `1000` passwords I generated was `148.616` using the zxcvbn algorithm (https://dropbox.tech/security/zxcvbn-realistic-password-strength-estimation), 
however feel free to test larger data sets.

## Usage
To use the password-generator, simply call `pw-gen`. It supports `STDOUT` so you pipe the outputted password to a file if you wish.
