# Timing Side Channel Attack Report
This report shows the steps we have been through before getting to the final result. According to the actual chronological order those steps have been performed, the main sections are:

* [Theoretical references](#theoretical-references)
  * [Side channel attack](#side-channel-attack)
  * [Montgomery based RSA encryption](#montgomery-based-rsa-encryption)
* [Code development](#code-development)
  * [Big integer library](#big-integer-library)
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

## Theoretical references

### Side channel attack
The name side channel attack refers to any attack based on a certain amount of information obtained from a computer system on which statistics could be computed. Relating those probabilistic statistics to the actual knowledge of the internal operation of the system, secrets related to the internal operations themselves could be disclosed.

In our case, the attack will be a timing attack on an RSA crypto-computation (Montgomery modular algorithm based ). This meas that, basing on the knowledge of the actual algorithm used by the encryption and exploiting its timing weaknesses, the attacker could obtain timing measurements on a series of encryptions and could analyze them, disclosing the secret key. More on the Montagomery based RSA ecryption is explained in the section [Montgomery based RSA encryption](#montgomery-based-rsa-encryption), while for the attack algorithm employed refer to [Attack algorithm](#attack-algorithm).

### Montgomery based RSA encryption
The RSA ecryption algorithm involves two steps:

* key pair generation
* modular exponentation and multiplication based ecryption

For the key pair generation, first two distinct large prime number (`p`,`q`) have to be found. Then, the modulus `n` is computed as the product of the two prime numbers. The Eulero's totient `t` is successively computed as the product

$`t = (p-1) x (q-1)`$

and the public exponent `e` is chosen such that

$`1 < e < t`$

and

$`gcd(e,t) = 1`$.

Finally, the secret exponent `d` is chosen such that

$`d \cdot e = 1 mod t`$.

The pair `(n,e)` constitutes the public key, while the pair `(n,d)` the secret one.

To perform the encryption of a message `m` to obtain the ciphertext `c`, the following operation is performed:

$`c = m^e mod n`$


## Code development

The main starting point to get a working implementation for the Montgomery based RSA encryption is having a library capable of managing integers on a large number of bits (such as 1024 or 2048), since this will be most likely the size that will be used by most of the main core variables (private and public key, for instance). Usually, standard C libraries support numbers up to 128 bits (long double), which is the minimum key size for an admissible time side channel attack on RSA encryption. Thus, an extra library is needed.

After such library is obtained, the pseudo-code presented above ([Theoretical references](#theoretical-references)) has to be ported into a real C implementation through the primary functions `Montgomery multiplication` and `Montgomery exponentation`.

Finally, both the library and the RSA encryption have to be checked against a reference and reliable implementation; in our case, it will be a Python one, since this programming language doesn't force any explicitly defined limit to a number object, which makes it an ideal candidate.

### Big integer library

### RSA ecryption

### Code validation


## Data acquisition

### Bare metal Zybo Board acquisition

### OS acquisition


## Attack

### Attack algorithm

### Attack results


## Countermeasures


## Conclusions


## Practical references
