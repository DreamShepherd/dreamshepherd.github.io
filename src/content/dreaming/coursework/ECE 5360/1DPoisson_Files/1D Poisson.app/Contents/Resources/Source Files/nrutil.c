#include <stdio.h>
#include <stddef.h>
#include <stdlib.h>
#define NR_END 1
#define FREE_ARG char*
void alertbox(char messag[]);
void storeAlert(char messag[]);

double *vector(long nl, long nh) {
	
	double *v;
	
	v = (double *)malloc((size_t) ((nh-nl+1+NR_END)*sizeof(double)));
	if (!v) alertbox("allocation failure in vector()");
	return v-nl+NR_END;
}

void free_vector(float *v, long nl, long nh) {
	
	free((FREE_ARG) (v+nl-NR_END));
}