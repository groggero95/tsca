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

#ifdef OMP
#include <omp.h>
#endif


/**
 *  \fn static inline void start_meas(time_stats_t *ts) __attribute__((always_inline))
 *  \brief Start the measure
 *   
 *  \details 
 *  Record the start time of one measure in the time_stats_t.in variable
 */
static inline void start_meas(time_stats_t *ts) __attribute__((always_inline));

/**
 *  \fn static inline void stop_meas(time_stats_t *ts) __attribute__((always_inline))
 *  \brief Stop the measure
 *   
 *  \details 
 *  Record the end time of one measure, computes the elapsed time and accumuate it in the time_stats_t.diff variable. \n
 *  Afterward it increment time_stats_t.trials by one and check if this was the samallest time recorded, in case updates the value of time_stats_t.min
 */
static inline void stop_meas(time_stats_t *ts) __attribute__((always_inline));

/**
 *  \fn static inline void reset_meas(time_stats_t *ts)
 *  \brief Reset the \ref time_stats_t structure
 *   
 *  \details 
 *  Initialize all members of \ref time_stats_t to zero exept time_stats_t.in, \n
 *  which does not need it as it is overwritten at the start of each measure.
 */
static inline void reset_meas(time_stats_t *ts);


/**
 *  \brief Structure to hold information about timing measuremnts
 *   
 *  \details 
 *  This structure is used hold timing measuremnt and retreive some useful information about them,
 *  like the minimun across multiple iteration and the average.
 */
typedef struct {
  long long in; /**< Holds the starting time of one measure */
  long long diff; /**< Accumulate the elapsed time between the beginnig and end of one measure */
  unsigned long long min; /**< Holds the minimum elapsed time measured so far */
  int trials; /**< Hold the number of mrasuremnt performed do far, it can be used yo derive the average time as \a diff/\a trials*/
} time_stats_t;

#if defined(__i386__)
static inline unsigned long long rdtsc_oai(void) __attribute__((always_inline));
static inline unsigned long long rdtsc_oai(void) {
    unsigned long long int x;
    __asm__ volatile (".byte 0x0f, 0x31" : "=A" (x));
    return x;
}
#elif defined(__x86_64__)
static inline unsigned long long rdtsc_oai() __attribute__((always_inline));
static inline unsigned long long rdtsc_oai() { 
  unsigned long long a, d;
  __asm__ volatile ("rdtsc" : "=a" (a), "=d" (d));
  return (d<<32) | a;
}
#endif

static inline void start_meas(time_stats_t *ts) {

#ifdef OMP
  int tid;

  tid = omp_get_thread_num();
  if (tid==0)
#endif
    {
      ts->trials++;
      ts->in = rdtsc_oai();
    }
}

static inline void stop_meas(time_stats_t *ts) {

  long long out = rdtsc_oai();

#ifdef OMP
  int tid;
  tid = omp_get_thread_num();
  if (tid==0)
#endif
    {
      ts->diff += (out-ts->in);
      if ((out-ts->in) < ts->min)
        ts->min = out-ts->in;
      
    }
}

static inline void reset_meas(time_stats_t *ts) {

  ts->trials=0;
  ts->diff=0;
  ts->min= ~0;
}
