# Elliptic Curve Cryptography and Secret Sharing

## Introduction

This C program demonstrates several operations related to elliptic curve cryptography (ECC) and secure multiparty computation. The key functionalities include:
1. Generating points on an elliptic curve.
2. Performing point addition and scalar multiplication on the elliptic curve.
3. Implementing elliptic curve Diffie-Hellman key exchange.
4. Computing additive secret shares for secure computation.

The elliptic curve used is defined by the equation \(y^2 = x^3 + ax + b \mod p\), with parameters chosen to resemble the secp256k1 curve, commonly used in cryptography.

## Getting Started

### Prerequisites

To compile and run this program, you will need:
- A C compiler (e.g., `gcc`).
- Basic understanding of C programming and elliptic curve cryptography.

### Files

- `ecc.c`: The main C program file containing all functions and the main routine.

### Compilation

Compile the program using a C compiler. For example, with `gcc`:

```sh
gcc -o ecc ecc.c -lm
```

### Usage

Run the program with two arguments representing the secret multiplicative shares:

```sh
./ecc <num1> <num2>
```

Example:

```sh
./ecc 12 15
```

## Program Structure

### Global Variables

- `a`, `b`, `p`: Parameters defining the elliptic curve.
- `generator[2]`: The generator point on the curve.
- `points[1000][2]`: Array to store points on the elliptic curve.
- `count`: Counter for the number of points on the curve.

### Functions

1. **generate_points**: Generates all points on the elliptic curve within the field defined by `p`.
2. **print_points**: Prints all the points generated on the elliptic curve.
3. **isPAI**: Checks if a point is the point at infinity.
4. **getPAI**: Returns the point at infinity.
5. **inverse**: Computes the modular inverse of a number.
6. **add**: Adds two points on the elliptic curve.
7. **sclr_mult**: Performs scalar multiplication of a point on the elliptic curve using the LSB-first algorithm.
8. **subtract**: Subtracts one point from another on the elliptic curve.
9. **calculate_additive_shares**: Computes additive shares for secure computation.

### Main Routine

The main routine performs the following steps:
1. Parses command-line arguments to get the secret multiplicative shares.
2. Generates points on the elliptic curve.
3. Randomly selects numbers for the key exchange process.
4. Computes points for the Diffie-Hellman key exchange and encryption.
5. Decrypts the messages using the shared secret key.
6. Computes additive shares for secure computation.

## Example Output

Example output for the provided code:

```sh
secret multiplicative share x: 12, y: 15 
Random number a: 42, Random number b: 76 
Point aG: (10, 3), Point bG: (7, 8)
Decrypted messages: (12, 15)
Alice's additive share (U): -1152
Bob's additive share (V): 1119
```
