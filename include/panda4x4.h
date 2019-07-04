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

#ifndef PANDA
#define PANDA

/**
 *  \hideinitializer
 *  \brief Number of bit of the exponent considered at each iteration of the attack.
 *   
 *  \details
 *  This constant determine the amount of guess that will be made for the next \a B_CONSIDERED bits. \n
 *  All these guesses will be tested using the \a pcc and only \ref B_GUESSED will be consolidated. \n
 *  The bigger this number the slower the attack will execute, since the amount of guess depends exponentially on it.
 */
#define B_CONSIDERED 	2

/**
 *  \hideinitializer
 *  \brief Number of bit of the exponent guessed at each iteration of the attack.
 *   
 *  \details
 *  This constant determine the amount of bit that I will consolidate as known at each iteration of the attack. \n
 *  In order for the attack to work we need: \n
 *
 *  	B_CONSIDERED >= B_GUESSED
 *
 *  Moreover as the diference increase between these two constant the more confidence we have on the bit thta we are guessing.
 */
#define B_GUESSED 		1

/**
 *  \brief Activate the attack on the multiply step.
 *   
 *  \details
 *  The attack is only possible on this step thus we need to leave it to \b 1. \n
 *  This option has been implemented only for demonstration porposes.
 */
#define ATTACK_MUL		1

/**
 *  \brief Activate the attack on the squaring step.
 *   
 *  \details
 *  Define if during the attack we want to estimate also the time taken by the sqyuaring step. \n
 *  In practice if activated it gives a non zero value to the \a pcc on a zero bit guess on the exponent. \n
 *  Without its contribution the attack may require from 3 to 5 times the amount of messages.
 */
#define ATTACK_SQUARE	1

/**
 *  \brief Coefficent used in filtering
 *   
 *  \details
 *  Define how many times, with respect to the standard deviation, a sample must be displaced from the mean 
 *  value of the distribution in order to be discarded by the fitering function.
 *  
 *  \remark Filtering is only relevant on the case of noisy measure derived from time acquisitions made on an operating system
 */
#define COEFF 			3

/**
 *  \brief Structure to hold information needed during the attack
 *   
 *  \details 
 *  This structure is used to hold the intermediate values \a c and \a s inside of a modular exponentiation. \n
 *  It also has a member to store the time estimate of the current and past iteration.
 *
 *  \sa ME_big, bigint_t, ME_big_estimate
 */
typedef struct msg_t
{
	bigint_t c; /**< Intermediate value for the output of the modular eponentiation*/
	bigint_t s; /**< Intermediate value of the message in Montgomery form*/
	uint32_t tot_est; /**< Accumulator for the time estimated to compute \a c and \a s at each iteration of the modular eponentiation*/
} msg_t;

#endif
