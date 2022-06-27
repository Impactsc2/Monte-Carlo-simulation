#include <iostream>
#include "initial_conditions.h"

using namespace std;

void triangle_network(double x[] , double y[] , double sigma[] , double N_x , double N_y) {

    int index = 0;
	for (int i = 0 ; i < N_x ; i++){
		for (int j = 0 ; j < N_y ; j++){
			
			x[index] =  i + 0.5*(j%2);
			y[index] =  j*0.866025404;
			sigma[index] = 1.0;//0.9 + 0.2*((double)rand() / (double)(RAND_MAX));
			index++;
		}
	}
}

void NaCl(double x[] , double y[] , double sigma[] , int N_x , int N_y , double L_x , double L_y) {

    int index = 0;
	for (int i = 0 ; i < N_x ; i++){
		for (int j = 0 ; j < N_y ; j++){
			
			x[index] =  i*(1.0*L_x/N_x) ;
			y[index] =  j*(1.0*L_y/N_y) ;
			sigma[index] = 0.5 + 0.5*((j + i)%2);
			index++;
		}
	}
}