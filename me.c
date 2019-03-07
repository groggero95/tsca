#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include "me.h"
#include "mm.h"

/* 	e  = exponent
	m  = message
	n  = modulus
	c0 = prcomputed value of: (1<<nb) % n
	s0 = prcomputed value of: m*(1<<nb) % n
	nb = number of bit
*/
int ME(int e, int m, int n, int c0, int s0, int nb){
	int c = c0;
	int s = s0; 

	for(int i=0, mask = 1; i < nb; i++,  mask =  mask << 1){
		if( e & mask ){
			c = MM(c,s,n,nb);
		}
		s = MM(s,s,n,nb);
	}
	c = MM(c,1,n,nb);
	return c;
};