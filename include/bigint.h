/*
 * Copyright (C) EURECOM, Telecom Paris
 *
 * Prof. Renaud PACALET         <renaud.pacalet@telecom-paristech.fr>
 * Alberto ANSELMO              <Alberto.Anselmo@eurecom.fr>
 * Simone Alessandro CHIABERTO	<Simone-Alessandro.Chiaberto@eurecom.fr>
 * Fausto CHIATANTE             <Fausto.Chiatante@eurecom.fr>
 * Giulio ROGGERO               <Giulio.Roggero@eurecom.fr>
 *
 * This file must be used under the terms of the CeCILL. This source
 * file is licensed as described in the file COPYING, which you should
 * have received as part of this distribution. The terms are also
 * available at:
 * http://www.cecill.info/licences/Licence_CeCILL_V1.1-US.txt
*/

#ifndef BIGINT_H
#define BIGINT_H

#include <stdint.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>

/**
 *  Nominal size (number of bit) of the bigint number.
 *  This value must be a power of two and the only supported values are:
 *  {64, 128, 256, 512, 1024, 2048, 4096}
 */
#define INT_SIZE 128

/**
 *  Size (number of bit) of the vector element componing a bigint
 *  This value must be within the following:
 *  {8, 16, 32, 64}
 */
#define VAR_SIZE 32

/**
 *  Length of the vector representing a bigint
 *  The +1 indicates that we always add @a VAR_SIZE bits to the nominal
 *  size defined in @a INT_SIZE. This is necessary to account for overflow and 
 *  for some intermediate value which may require more bits before being reduced 
 *  again to the nominal size.
 */
#define NUMB_SIZE INT_SIZE/VAR_SIZE + 1

/**
 *  Number of hexadecimal 
 */
#define HEX_DIGIT VAR_SIZE/4

/**
 *  Definition of some constant string used for initialization of @a bigint_t data
 *  Limited version are available in relation of the nominal size of the integers
 */
#if INT_SIZE == 64
static const char ONE[] = "0x0000000000000001";
static const char ZERO[] = "0x0000000000000000";
#elif INT_SIZE == 128
static const char ONE[] = "0x00000000000000000000000000000001";
static const char ZERO[] = "0x00000000000000000000000000000000";
#elif INT_SIZE == 256
static const char ONE[] = "0x0000000000000000000000000000000000000000000000000000000000000001";
static const char ZERO[] = "0x0000000000000000000000000000000000000000000000000000000000000000";
#elif INT_SIZE == 512
static const char ONE[] = "0x00000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000001";
static const char ZERO[] = "0x00000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000";
#elif INT_SIZE == 1024
static const char ONE[] = "0x0000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000001";
static const char ZERO[] = "0x0000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000";
#elif INT_SIZE == 2048
static const char ONE[] = "0x00000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000001";
static const char ZERO[] = "0x00000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000";
#elif INT_SIZE == 4096
static const char ONE[] = "0x0000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000001";
static const char ZERO[] = "0x0000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000";
#endif

/**
 *  Definition of the type used for the vector rapresentation of large size integers
 *  and some related bitwise masks
 */
#if VAR_SIZE == 64
    #define UMAX 0xFFFFFFFFFFFFFFFF
    #define HIMASK 0xFFFFFFFF00000000
    #define LOMASK 0x00000000FFFFFFFF
    typedef uint64_t var_t;
#elif VAR_SIZE == 32
    #define UMAX 0xFFFFFFFF
    #define HIMASK 0xFFFF0000
    #define LOMASK 0x0000FFFF
    typedef uint32_t var_t;
#elif VAR_SIZE == 16
    #define UMAX 0xFFFF
    #define HIMASK 0xFF00
    #define LOMASK 0x00FF
    typedef uint16_t var_t;
#elif VAR_SIZE == 8
    #define UMAX 0xFF
    #define HIMASK 0xF0
    #define LOMASK 0x0F
    typedef unit8_t var_t;
#endif


/**
 *  @brief Structure to hold large fixed size unsigned integer 
 */
typedef struct bigint{
    var_t numb[NUMB_SIZE]; /**< number with high bit number in vector format */
} bigint_t;


/**
 *  @brief Equality test
 *
 *  @details
 *   Compares two number and decide if they are equal  
 *
 *  @param a  First argument
 *  @param b  Second argument
 *  @return   True or false in an integer format, i.e. as 1 or 0
 */
int eq(bigint_t a, bigint_t b);

/**
 *  @brief Inequality test
 *
 *  @details
 *   Compares two number and decide if they are different  
 *
 *  @param a  First argument
 *  @param b  Second argument
 *  @return   True or false in an integer format, i.e. as 1 or 0
 */
int df(bigint_t a, bigint_t b);

/**
 *  @brief Compares two number
 *
 *  @details
 *   Compares two number @a a and @a b and decides if @a a is greater than @a b 
 *
 *  @param a  First argument
 *  @param b  Second argument
 *  @return   True or false in an integer format, i.e. as 1 or 0
 */
int gt(bigint_t a, bigint_t b);

/**
 *  @brief Compares two number
 *
 *  @details
 *   Compares two number @a a and @a b and decides if @a a is greater than or equal to @a b 
 *
 *  @param a  First argument
 *  @param b  Second argument
 *  @return   True or false in an integer format, i.e. as 1 or 0
 */
int ge(bigint_t a, bigint_t b);

/**
 *  @brief Compares two number
 *
 *  @details
 *   Compares two number @a a and @a b and decides if @a a is less than @a b 
 *
 *  @param a  First argument
 *  @param b  Second argument
 *  @return   True or false in an integer format, i.e. as 1 or 0
 */
int lt(bigint_t a, bigint_t b);

/**
 *  @brief Compares two number
 *
 *  @details
 *   Compares two number @a a and @a b and decides if @a a is less than or equal to @a b 
 *
 *  @param a  First argument
 *  @param b  Second argument
 *  @return   True or false in an integer format, i.e. as 1 or 0
 */
int le(bigint_t a, bigint_t b);

/**
 *  @brief Bitwise and
 *
 *  @details
 *   Compute the bitwise and between two number @a a and @a b 
 *
 *  @param a  First argument
 *  @param b  Second argument
 *  @return   Bitwise and of the two argumnts
 */
bigint_t and(bigint_t a, bigint_t b);

/**
 *  @brief Bitwise or
 *
 *  @details
 *   Compute the bitwise or between two number @a a and @a b 
 *
 *  @param a  First argument
 *  @param b  Second argument
 *  @return   Bitwise or of the two argumnts
 */
bigint_t or(bigint_t a, bigint_t b);

/**
 *  @brief Bitwise not
 *
 *  @details
 *   Compute the bitwise not of a number 
 *
 *  @param a  Input argument
 *  @return   Bitwise not of the argumnt
 */
bigint_t not(bigint_t a);

/**
 *  @brief Bitwise xor
 *
 *  @details
 *   Compute the bitwise xor between two number @a a and @a b 
 *
 *  @param a  First argument
 *  @param b  Second argument
 *  @return   Bitwise xor of the two argumnts
 */
bigint_t xor(bigint_t a, bigint_t b);

/**
 *  @brief Logical shift right
 *
 *  @details
 *   Shift bit a bit of a number @a a by a specific amount @a sh to the right.
 *   For each bit leving the number @a a the most significant bit is zero filled.
 *
 *  @param a   Input number
 *  @param sh  Shift amount
 *  @return    Number @a a shifted by @a sh bit to the right
 */
bigint_t lsr(bigint_t a, int sh);

/**
 *  @brief Logical shift left
 *
 *  @details
 *   Shift bit a bit of a number @a a by a specific amount @a sh to the left.
 *   For each bit leving the number @a a the least significant bit is zero filled.
 *
 *  @param a   Input number
 *  @param sh  Shift amount
 *  @return    Number @a a shifted by @a sh bit to the right
 */
bigint_t lsl(bigint_t a, int sh);

/**
 *  @brief Sum operation
 *
 *  @details
 *   Perform the sum between two number of @a bigint_t data type.
 *   Overflow may happen as the size of @a bigint_t is finite
 *
 *  @param a  First argument
 *  @param b  Second argument
 *  @return   Sum between @a a and @a b
 */
bigint_t sum(bigint_t a, bigint_t b);

/**
 *  @brief Subtraction operation
 *
 *  @details
 *   Perform the subtarction between two number of @a bigint_t data type.
 *   Overflow may happen as the size of @a bigint_t is finite
 *
 *  @param a  First argument
 *  @param b  Second argument
 *  @return   Subtarction between @a a and @a b
 */
bigint_t sub(bigint_t a, bigint_t b);


/**
 *  @brief Multiplication operation
 *
 *  @details
 *   Perform the multiplication between two number of @a bigint_t data type.
 *   The outcome will be represented on the same number of bits of the input
 *   thus it could be truncated.
 *
 *  @param a  First argument
 *  @param b  Second argument
 *  @return   Multiplication between @a a and @a b truncated to the size of @a bigint_t data type.
 */
bigint_t mul(bigint_t a, bigint_t b);


/**
 *  @brief Initialize a @a bigint_t from a string
 *
 *  @details
 *   Generate a bigint number sarting from its hexadecimal rapresentation
 *   in string format
 *
 *  @param[in] s  Hexadecimal number in string format, zero padded to the nominal length
 *  @return       Bigint verson of the input
 */
bigint_t init(const char *s);

/**
 *  @brief Initialize a @a bigint_t from a string
 *
 *  @details
 *   Generate a bigint number sarting from its hexadecimal rapresentation
 *   in string format
 *
 *  @param[in] s  Hexadecimal number in string format, zero padded to the full length
 *  @return       Bigint verson of the input
 */
bigint_t init_full(const char *s);

/**
 *  @brief Sum operation used inside the multiplication
 *
 *  @details
 *   Sum between two integer with and optional carry in as input.
 *   Used internally to the mul operatiom to perform the intermediate summation 
 *   between the various partial multiplication.
 *
 *  @param[in, out] a  First operand of the summation, will also hold the result
 *  @param b           Second operand of the summation
 *  @param[in] carry   Carry In, kept in consideration only it @a act \= 1
 *  @param act         Decide if the carry in should be added or discarded
 *  @return            Carry output of the summation
 */
var_t sum_4_mul(var_t *a, var_t b, var_t *carry, int act);

/**
 *  @brief Print a bigint in hexadecimal format
 *
 *  @details
 *   Print a bigint to the standard output in hexadecimal format.
 *   Mainly used for visual debugging and testing pourposes
 *   
 *
 *  @param[in] a  Hexadecimal number in string format, zero padded to the full length
 *  @return       Always return @c NULL
 */
void print_to_stdout(bigint_t *a);

/**
 *  @brief Generate a random bigint
 *
 *  @details
 *   Generate a random bigint of the current nominal size
 *
 *  @return  Random bigint
 */
bigint_t rand_b( void );

#endif
    