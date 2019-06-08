# Timing Side Channel Attack Report
This report shows the steps we have been through before getting to the final result. According to the actual chronological order those steps have been performed, the main sections are:

* [Theoretical references](#theoretical-references)
  * [Side channel attack](#side-channel-attack)
  * [Montgomery based RSA encryption](#montgomery-based-rsa-encryption)
* [Code development](#code-development)
  * [Big integer library](#big-integer-library)
    * [Data type](#data-type)
    * [Operations](#operations)
  * [RSA ecryption](#rsa-ecryption)
  * [Code validation](#code-validation)
* [Data acquisition](#data-acquisition)
  * [Bare metal Zybo Board acquisition](bare-metal-zybo-board-acquisition)
  * [OS acquisition](os-acquisition)
* [Attack](#attack)
  * [Attack algorithm](#attack-algorithm)
  * [Attack results](#attack-results)
* [Countermeasures](#countermeasures)
* [Conclusions](#conclusions)
* [Practical references](#practical-references)

Before starting, clone the git repository and get into it:

```bash
$ git config --global user.name "John Doe"
$ git config --global user.email johndoe@example.com
$ cd some-where
$ git clone git@gitlab.eurecom.fr:roggero/tsca.git
$ cd tsca
```

## Theoretical references

### Side channel attack
The name side channel attack refers to any attack based on a certain amount of information obtained from a computer system on which statistics could be computed. Relating those probabilistic statistics to the actual knowledge of the internal operation of the system, secrets related to the internal operations themselves could be disclosed.

In our case, the attack will be a timing attack on an RSA crypto-computation (Montgomery modular algorithm based ). This meas that, basing on the knowledge of the actual algorithm used by the encryption and exploiting its timing weaknesses, the attacker could obtain timing measurements on a series of encryptions and could analyze them, disclosing the secret key. More on the Montagomery based RSA ecryption is explained in the section [Montgomery based RSA encryption](#montgomery-based-rsa-encryption), while for the attack algorithm employed refer to [Attack algorithm](#attack-algorithm).

### Montgomery based RSA encryption
The RSA ecryption algorithm involves two steps:

* key pair generation
* modular exponentation and multiplication based ecryption

For the key pair generation, first two distinct large prime number (`p`,`q`) have to be found. Then, the modulus `n` is computed as the product of the two prime numbers. The Eulero's totient `t` is successively computed as the product

$`t = (p-1) \cdot (q-1)`$

and the public exponent `e` is chosen such that

$`1 < e < t`$

and

$`gcd(e,t) = 1`$ .

Finally, the secret exponent `d` is chosen such that

$`d \cdot e = 1 \; mod \; t`$ .

The pair `(n,e)` constitutes the public key, while the pair `(n,d)` the secret one.

To perform the encryption of a message `m` to obtain the ciphertext `c`, the following operation is performed:

$`c = m^e \; mod \; n`$ .

This computation consists of two main operations: modular multiplication and exponentiation. The implementation we adopted takes advantage instead of the Montgomery multiplication: the multiplier digits are consumed in the reverse order and no full length comparisons are required for the modular reductions (refer to [Colin D. Walter paper] on the topic). The basic pseudo-code it defines for the Montgomery modular multiplication is:

```text
S = 0;
for i = 0 to nb−1 do
  qi  = (s0 + aib0 )(-m^-1 ) mod r;
  S   = (S + ai × b + qi × n) div r;
end for;
return S;
```

where `nb` is the total number of bits of the secret key, `a` and `b` are the first two operands which are determined according to the Montgomery exponentation function:

```text
c = MM(k0,1,n);
s = MM(k0,m,n);
for i = 0 to nb−1 do
  if (ei = 1) then
    c = MM(c,s,n);  [multiply]
  end if;
  s = MM(s,s,n);    [square]
end for;
c = MM(c,1,n);
return c;
```

where `MM()` is the Montgomery multiplication defined by the previous algorithm and `k0` is

$`k_0 = 2^n`$

where `n` is the modulus computed before.

## Code development

The starting point to get a working implementation for the Montgomery based RSA encryption is having a library capable of managing integers on a large number of bits (such as 1024 or 2048), since this will be most likely the size that will be used by most of the main core variables (private and public key, for instance). Usually, standard C libraries support numbers up to 128 bits (long double), which is the minimum key size for an admissible time side channel attack on RSA encryption. Thus, an extra library is needed.

After such library is obtained, the pseudo-code presented above ([Theoretical references](#theoretical-references)) has to be ported into a real C implementation through the primary functions `Montgomery multiplication` and `Montgomery exponentation`.

Finally, both the library and the RSA encryption have to be checked against a reference and reliable implementation; in our case, it will be a Python one, since this programming language doesn't force any explicitly defined limit to a number object, which makes it an ideal candidate.

### Big integer library

There are two main possibilities to manage large integer numbers: rely on an available library (such as `GMP` (GNU Multiple Precision Arithmetic Library) or `OpenSSL`) or create one from scratch.

The former implies to understand how it works and to deal with an optimized version of all the main operations. This code optimizations could lead to a significant reduction of the execution time related to data dependencies, making the final attack more complicated.

The latter, on the other side, requires more time to be implemented and tested, but theoretically should guarantee a higher data dependency. Thus, this option is the one used.  

The custom library is implemented through the files [bigint.h] and [bigint.c], which define:
* the data type we will use to work on large integers
* all the main operation needed to perform the Montgomery multiplication and exponentation.

#### Data type

In the file [bigint.h] the following parameter are free to be set:

  * `VAR_SIZE`: it determines the basic unit to build the larger integer among 8 (uint8_t), 16 (uint16_t), 32 (uint32_t) and 64 (uint64_t) bits. Recommended size is 32.
  * `INT_SIZE`: it determines the actual length of the data the system is working on (for instance, public and private key dimensions). Possible sizes are 64, 128, 256, 512, 1024 and 2048 bits.

As a consequence of these two parameter, the code defines the `bigint_t` data type as a struct containing a vector of `NUMB_SIZE` elements of size `VAR_SIZE`, where `NUMB_SIZE` is equal to

$`NUMB\_SIZE = \frac{INT\_SIZE}{VAR\_SIZE} + 1`$ .

Thus, the vector will always have and extra element, used to store possible carries due to intermediate operations. Instead of an extra element `VAR_SIZE` long, a couple of bits would have been enough but, to keep the operations implementation simpler and straightforward, the choice fell back on the first solution.

Data are saved in little endian format, i.e. the lowest address in the array contains the lowest chunk of data, and so on.

#### Operations

All the operations use parameter passed by value (no pointer usage). The library contains the following operations:

* Comparisons (if `first` is `eq`/`df`/`gt`/`ge`/`lt`/`le` to/than `second`, then the comparison returns `1` (true), otherwise returns `0` (false)):
  * Equality:
    * int eq(bigint_t first, bigint_t second);
  * Diversity:
    * int df(bigint_t first, bigint_t second);
  * Greater than:
    * int gt(bigint_t first, bigint_t second);
  * Greater or equal:
    * int ge(bigint_t first, bigint_t second);
  * Lower than:
    * int lt(bigint_t first, bigint_t second);
  * Lower or equal:
    * int le(bigint_t first, bigint_t second);


* Logicals (bitwise operation between `a` and `b`, except the `not`, which reverts `a`):
  * Bitwise and:
    * bigint_t and(bigint_t a, bigint_t b);
  * Bitwise or:
    * bigint_t or(bigint_t a, bigint_t b);
  * Not:
    * bigint_t not(bigint_t a);
  * Bitwise xor:
    * bigint_t xor(bigint_t a, bigint_t b);


* Shifts (bigint_t `a` is shifted of `pl` positions (logical shift semantic)):
  * Logical shift right:
    * bigint_t lsr(bigint_t a, int pl);
  * Logical shift left:
    * bigint_t lsl(bigint_t a, int pl);


* Arithmetics (`a` is `sum`/`sub`/`mul` with `b`):
  * Sum:
    * bigint_t sum(bigint_t a, bigint_t b);
  * Subtraction:
    * bigint_t sub(bigint_t a, bigint_t b);
  * Multiplication (the result is casted to the `bigint_t` size):
    * bigint_t mul(bigint_t a, bigint_t b);


* Utility:
  * Init: initialize a variable into the `bigint_t` structure passed a pointer, except the extra element (element `NUMB\_SIZE`):
    * bigint_t init(const char \*s);
  * Init_full: as `init`, but initialize also the extra element:
    * bigint_t init_full(const char \*s);
  * Sum_4_mul: special sum for the `mul` operation:
    * var_t sum_4_mul(var_t \*a, var_t b, var_t \*carry, int act);
  * print_to_stdout: print the `bigint_t` number in hexadecimal format (0x..):
    * void print_to_stdout(bigint_t \*a);
  * rand_b: return a random `bigint_t` number:
    * bigint_t rand_b( void );

To check the actual implemetations of those functions, refer to the file [bigint.c].

### RSA ecryption

The Montgomery multiplication and exponentiation pseudo-codes (section [Montgomery based RSA encryption](#montgomery-based-rsa-encryption)) are ported instruction by instruction in C with the implemetations reported in the files pair [mm.h], [mm.c] and [me.h], [me.c]. More specifically, refer to the functions `MM_big` and `ME_big`.

### Code validation

The `bigint` library and the Montgomery exponentation are now ready to be tested. As previously mentioned, Python is the programming language chosen both to launch several times the test operation and to provided the reference implementation. To compile the code, issue the following commands:

```bash
$ cd tsca
$ make test
```

It will generate a file called `main` in the same folder. Enter then the folder `test`:

```bash
$ cd test
```

It contains a set of python test programs which implement reliable versions of the very same C functions listed in C (section [Operations](#operations)). To run the tests on the `bigint` library, type:

```bash
# Comparisons:
$ python3 comp.py <operation> <numberoftests> <bit>
# Logicals:
$ python3 logic.py <operation> <numberoftests> <bit>
# Shifts:
$ python3 shift.py <operation> <numberoftests> <bit>
# Arithmetics:
$ python3 arith.py <operation> <numberoftests> <bit>
```

The chosen python script will check the custom implementation launching the executabe `main` aginst its intenal implementation. If `<numberoftest>` and `<bit>` are not defined, the program will automatically test for 10000 tests on 128 bits.

To run instead the test on the Montomery operations type:

```bash
# Multiplication
$ python3 modular.py mm <numberoftests> <bit>
# Exponentation
$ python3 modular.py me <numberoftests> <bit>
```

All the functions have been tested with the approach just shown for a number of tests between 10 millions and 100 millions each. Each iteration of any of the Python scripts uses random numbers generated runtime. Since at the time this report is written no error is detected, the library are supposed to be reliable from now on.

## Data acquisition

It's time now to intensively run many Montgomery exponentation encryption on a bunch of messages using different sets of private exponent `e`, modolus `m` and `k0` and obtain the timing measurements associated to each set, to be able afterwards to mount an attack on them.

The different predefined sets are declared in the file [cipher.c]:  the values for `VERSION` in [cipher.h] and having a predefined value for the key width (set in [bigint.h] with the parameter `INT_SIZE`) picks up a different set. The number of sets is limited since we don't have a code capable of generating them autonomously. Have a look at them and select one.

Two different codes are available to obtain timing measurements:

* [helloworld.c]: to get measurements on the zybo board without the intervention of an OS;
* [timing.c]: to get measurement on a pc/laptop running an OS.


### Bare metal Zybo Board acquisition

The folder [zybo] contains all the necessary files to 

To run acquisitions on a OS-less system, in our case the Zybo board, two preliminary steps are necessary:

* set the `VERSION` and the `INT_SIZE` as explained before;
*

### OS acquisition


## Attack

### Attack algorithm

### Attack results


## Countermeasures


## Conclusions


## Practical references



[Colin D. Walter paper]: ./docs/CDW_ELL_99.pdf
[bigint.h]: ./include/bigint.h
[mm.h]: ./include/mm.h
[me.h]: ./include/me.h
[bigint.c]: ./source/bigint.c
[mm.c]: ./source/mm.c
[me.c]: ./source/me.c
[cipher.c]: ./source/cipher.c
[cipher.h]: ./source/cipher.h
[helloworld.c]: ./zybo/Test_sd/Debug
[timing.c]: ./source/timing.c
[zybo]: ./zybo/
