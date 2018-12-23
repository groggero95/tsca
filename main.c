#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <stdint.h>
#define n 16
#define w 1
#define r (1 << w)
#define s (n/w)
#define R (1 << s)

int MM(int a, int b, int m, int nb);



int main(int argc, char* argv[]){

	// m and R should be coprime because we want the multiplicative inverse of m (m^(-1)) unique
	// inside the ring of number {0 < k < m} at least from my understanding
	// moreover since we use 2 as the base of our numbers (i.e. in binary form) (m0^(-1)) is 
	// limited to the value of 1, which means that m is an odd number
	int m = 11;
	int a = 27;
	int b = 52;
	int a_ = (a << s) % m;
	int b_ = (b << s) % m;
	int a_x_b_ = (a*R*b) % m;

	printf("a: %d, b: %d, m: %d\n",a,b,m);
	printf("a*R mod m: %d, b*R mod m: %d\n",a_,b_);
	int res = MM(a_,b_,m,n);

	printf("a_x_b_: %d, MM: %d\n",a_x_b_,res);
	
	int i_,j_;
	uint64_t i_x_j_;
	for(int i = 2; i < 182; i++ ){
		for(int j = 2; j < 182; j++ ){
			i_x_j_ = (i*R*j);
			//printf(" a: %d, b: %d, i_x_j_: %lu\n",i,j,i_x_j_);
			i_x_j_ = i_x_j_ % m;
			i_ = (i << s) % m;
			j_ = (j << s) % m;
			res = MM(i_,j_,m,n);
			if(res != i_x_j_){
				printf("ERROR for a: %d, b: %d, i_x_j_: %ld, res: %d\n",i,j,i_x_j_,res);
			}
		}
	}


	return 0;

}

int MM(int a, int b, int m, int nb){

	//printf("a: %d, b: %d, m: %d, nb: %d\n",a,b,m,nb );
	int b0 = b & 0x01;
	int res = 0;
	for( int i = 0, mask = 1, ai, qi, res0; i < nb; i++, mask =  mask << 1){
		ai = (a & mask) >> i;
		res0 = res & 0x01;
		qi = (res0 + ai*b0) % r;
		res = (res + ai*b + qi*m) >> 1;
		//printf("q%d: %d, a%d: %d, s: %d\n",i, qi, i, ai,res);

	}

	return res;

}

