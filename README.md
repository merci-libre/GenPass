# genpass v1.3
Generates a strong password in ASCII up to 256 characters. (BUILT FOR *NIX and MacOS SYSTEMS!)

## Building
To build the base program, run:
```
gcc main.c -o genpass
```
I also included a quick program made by ChatGPT to calculate the average of my bit entropy `dataset`, if you want to verify the average you can
compile the `average_calculator.c` program.

## About
Pretty much, the program generates a long string from a matrix of numbers that represent ASCII characters.
The average bit entropy of the `1000` passwords I generated at length 25 was `148.616` using the zxcvbn algorithm (https://dropbox.tech/security/zxcvbn-realistic-password-strength-estimation), 
however feel free to test larger data sets. I also included a generated list from KeePassXC's password generating algorithm, just for users to verify certain statistics.

The weakness compared to KPXC's algorithm is about 3-4% weaker in regards to bit entropy, but has a lower time complexity.

## Usage
To use the password-generator, simply call `genpass -l <LENGTH>`. It supports `STDOUT` so you can pipe the outputted password to a file if you wish.
`genpass -l 20` will generate a password from ASCII values `32->126`.  However, you can also invoke command-line arguments that can change the characters used in the string.

```
: Command-Line Arguments :
-c : Removes spaces from generated strings.
-u : Uppercase characters only.
-x : Lowercase characters only.
-m : Mix of uppercase and lowercase characters.
-n : Numeric characters only.
```
For example, `genpass -n -l 20` would generate a string of numbers with the length of 20.

## Windows support
Feel free to fork the program with a precompiled Windows binary included. I might create one eventually, but it's not particularly high on my 'todo list.'
Some notes for developers:
```
    1. The pseudorandom number generation in the Gordion Knot Algorithm uses the PID in it's seed generation. This is the only function that requires a system library.
    2. You may also want to include a GUI for Windows as I have 0 experience in UI/UX design, which is why I intended this software to be used as a CLI program.
```
