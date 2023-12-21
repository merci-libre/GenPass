# genpass
Generates a strong password in ASCII up to 256 characters. (BUILT FOR *NIX and MacOS SYSTEMS!)

## Building
To build the base program:
```gcc main.c -o genpass```
I also included a quick program made by ChatGPT to calculate the average of my bit entropy `dataset`, if you want to verify the average you can
compile the `average_calculator.c` program.

## About
Pretty much, the program generates a long string from a matrix of numbers that represent ASCII characters.
The average bit entropy of the `1000` passwords I generated at length 25 was `148.616` using the zxcvbn algorithm (https://dropbox.tech/security/zxcvbn-realistic-password-strength-estimation), 
however feel free to test larger data sets. I also included a generated list from KeePassXC's password generating algorithm, just for users to verify certain statistics.

## Usage
To use the password-generator, simply call `genpass -l <LENGTH>`. It supports `STDOUT` so you pipe the outputted password to a file if you wish.
