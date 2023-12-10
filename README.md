# The Project: Cryptography
```
3rd year Epitech Project.
The goal of this project is to code several type of encryption and decryption.
```
# Usage
```
./mypgp -h USAGE
      ./mypgp [-xor | -aes | -rsa | -pgp] [-c | -d] [-b] KEY
      the MESSAGE is read from standard input
DESCRIPTION
  - xor computation using XOR algorithm
  -aes computation using AES algorithm
  -rsa computation using RSA algorithm
  -pgp computation using both RSA and AES algorithm
  -c MESSAGE is clear and we want to cipher it
  -d MESSAGE is ciphered and we want to decipher it
  -b block mode: for xor and aes, only works on one block MESSAGE and KEY must be of the same size
  -gP Q for RSA only: generate a public and private key pair from the prime number P and Q
```
