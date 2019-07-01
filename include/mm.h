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

#include "bigint.h"

#ifndef MM_H
#define MM_H

/**
 *  Constants used on the mm function 
 */
#define k 1
static const int tr[] = {0, ~0};

/**
 *  @brief Modular Exponentiation using integers
 *
 *  @details
 *   Performs @f$ a \cdot b \cdot R^{-1} \cmod n @f$, this operation is called 
 *   Montgomery modular multiplication. This type of operation naturally add the
 *   factor @f$  R^{-1} @f$ in the modular multiplication between two number @a a and @a b.
 *   The constant @a R is equal to @f$ r^{nb} @f$ where @a r is the base in which we are 
 *   representing our number (2 in our case) and @a nb is the number of bit of our operands.
 *   Another important iformation is that -1 exponent, which in modular arithmetic represent
 *   the modulr inverse for the multiplication operation, i.e. @f$ R \cdot R^{-1} \cmod n = 1 @f$.
 * 	 A number @a a is said to be in Montgomery form when multiplied by the factor @a R.
 *   
 *   This function was used for testing the validity of the algorithm on a manageble
 *	 number of bits. It does not have any other pourpose.
 *
 *  @param a   First operand
 *  @param b   Second Operand
 *  @param n   Modulus, for correct functioning gcd(n,R) = 1
 *  @param nb  Number of bit of the input oprenads 
 *  @return    The result of the Montgomery modular exponentiation
 */
int mm(unsigned int a, unsigned int b, unsigned int n, unsigned int nb);

/**
 *  @brief Modular Exponentiation using integers
 *
 *  @details
 *   Performs @f$ a \cdot b \cdor R^{-1} \cmod n @f$, this operation is called 
 *   Montgomery modular multiplication. This type of operation naturally add the
 *   factor @f$  R^{-1} @f$ in the modular multiplication between two number @a a and @a b.
 *   The constant @a R is equal to @f$ r^{nb} @f$ where @a r is the base in which we are 
 *   representing our number (2 in our case) and @a nb is the number of bit of our operands.
 *   Another important iformation is that -1 exponent, which in modular arithmetic represent
 *   the modulr inverse for the multiplication operation, i.e. @f$ R \cdot R^{-1} \cmod n = 1 @f$.
 * 	 A number @a a is said to be in Montgomery form when multiplied by the factor @a R.
 *   
 *   The iterative part of this function is dependent on the number of bits of the operandns and
 *   in order to avoid some comarison at the and, two leading zero bit are assumed in each
 *   operand and two more iteration are performed. In this way we assure the correctness of the
 *   output, i.e. thta the output is strictly less then the modulus n 
 *
 *  @param a   First operand
 *  @param b   Second Operand
 *  @param n   Modulus, for correct functioning gcd(n,R) = 1
 *  @param nb  Number of bit of the input oprenads 
 *  @return    The result of the Montgomery modular exponentiation
 */
bigint_t MM_big(bigint_t a, bigint_t b, bigint_t n, int nb);

/**
 *  @brief Estimate the time employed by a Montgomery modular multiplication
 *
 *  @details
 *   Performs the Montgomery modular multiplication algorithm counting
 *   the number of branch taken. This amount is accumulated on the variable
 *   cnt and can be used by an attacker as a timing model for the function.
 *
 *  @param a             First operand
 *  @param b             Second Operand
 *  @param n             Modulus, for correct functioning gcd(n,R) = 1
 *  @param nb  	         Number of bit of the input oprenads 
 *  @param cnt[in, out]  Accumulate the number of branch taken in the algorithm
 *  @return              The result of the Montgomery modular exponentiation
 */
bigint_t MM_big_estimate(bigint_t a, bigint_t b, bigint_t n, int nb, uint32_t *cnt);

#endif
