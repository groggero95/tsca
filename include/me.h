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

#ifndef ME_H
#define ME_H

#include "panda4x4.h"

/**
 *  @brief Modular Exponentiation using integers
 *
 *  @details
 *   Performs @f$ m^e \cmod n @f$, this operation is called modular exponentiation.
 *	 Instead of computing directly such operation we exploit the propery of modular
 * 	 arithmetic to iteratively compute the outcome of this operation without the need
 *   of a very large amount of bit in order to represent the result of the power.
 *   The algorithm is based on square and multiply operation which are both performed
 *   using the Montgomery multiplication.
 *  
 *   For the correct functioning of the function the base must be at maximum 30 bit 
 *   in order to avoid overflow in the intermediate operation performed inside the function. 
 *    
 *   This function was used for testing the validity of the algorithm on a manageble
 *	 number of bits. It does not have any other pourpose. Moreover the inputs are different
 *   than the standard one used later
 *
 *  @param e   Secret or public key, used as exponent for the power
 *  @param n   Modulus, must be an odd number for the correctness of the function
 *  @param c0  Number one in Montgomery domain, i.e. @f$ 1 \cdot R \cmod n @f$, it is 
 *    used as starting point to compute the output of the function 
 *  @param s0  Message @a m in Montgomery domain, i.e. @f$ m \cdot R \cmod n @f$, it 
 *    is this @a m which should be less than 30 bit
 *  @return   The result of the modular exponentiation
 */
int me(unsigned int e, unsigned int n, unsigned int c0, unsigned int s0, unsigned int nb);

/**
 *  @brief Modular Exponentiation using bigint
 *
 *  @details
 *   Performs @f$ m^e \cmod n @f$, this operation is called modular exponentiation.
 *	 Instead of computing directly such operation we exploit the propery of modular
 * 	 arithmetic to iteratively compute the outcome of this operation without the need
 *   of a very large amount of bit in order to represent the result of the power.
 *   The algorithm is based on square and multiply operation which are both performed
 *   using the Montgomery multiplication. Moreover it belongs to the class denoted as 
 *   RL, since it cosumes the bits of the exponent from Right to Left, i.e. from the 
 * 	 least significant to the most significant bit.
 *  
 *
 *  @param e   Secret or public key, used as exponent for the power
 *  @param n   Modulus, must be an odd number for the correctness of the function
 *  @param k0  Constant related to the modulus @a n. It is used to convert a number
 *    into its Montgomery representation by means of a Montgomery multiplication.
 *    This parameter is computed as follow: @f$ R^2 \cmod n @f$
 *  @return   The result of the modular exponentiation
 */
bigint_t ME_big(bigint_t e, bigint_t m, bigint_t n, bigint_t k0, int nb);

/**
 *  @brief Modular Exponentiation with blinding countermeasure
 *
 *  @details
 *   Performs a modular exponentiation but it implenets blinding
 *   in it to prevent timing side channel attack.
 *   Internally the function modify the input to the algorithm
 *   used to perform the modular eponantiation and then undo such
 *   modification to retrive the correct output. In this way we
 *   are hiding the real inputs of the function to an attacker,
 *   making thus the function less prone to leak information.
 *  
 *
 *  @param e   Secret or public key, used as exponent for the power
 *  @param n   Modulus, must be an odd number for the correctness of the function
 *  @param k0  Constant related to the modulus @a n. It is used to convert a number
 *    into its Montgomery representation by means of a Montgomery multiplication.
 *    This parameter is computed as follow: @f$ R^2 \cmod n @f$
 *  @return   The result of the modular exponentiation
 */
bigint_t ME_big_blind(bigint_t e, bigint_t m, bigint_t n, bigint_t k0, int nb);

/**
 *  @brief Estimate the time employed by part of the modular exponentiation 
 *
 *  @details
 *   Performs @a bits_step iteration of the modular exponentiation, estimate the
 *   time taken and accumulate this estimate with the estimate of the previous steps.
 *   The timing model employed by this function is simply the number of branch taken
 *   while performing the intermediate operations of square and multiply.
 *  
 *
 *  @param bits        Contains the bit sequence of the exponent for which I want to 
 *    execute the modular exponentiation algorithm
 *  @param m[in, out]  Struct containing the intermediate values resulted from the
 *    previous cycles of the modular exponentiation algorithm and the time estimated
 *    for those step and all the previous one
 *  @param n           Modulus, must be an odd number for the correctness of the function
 *  @param bits_step   Number of cycle of the modular exponentiation algorithm which I want to perform
 *  @return            Always return @c NULL
 */
void ME_big_estimate(uint32_t bits, msg_t *m, bigint_t n, int bits_step);

#endif