#include <iostream>
#include <fstream>
#include <string>
#include <math.h>
#include <time.h>
#include <stdlib.h>
#include "initial_conditions.h"

using namespace std;

double* x = NULL;
double* y = NULL;
double* sigma = NULL;
double L_x = 0;
double L_y = 0;
double VVcp = 0;
int N = 0;
int N_x = 0;
int N_y = 0;
double R_Max = 0;
double acceptance_ratio;
int ilosc_cykli = 0;
int accepted = 0;
double all_accepted = 0;
string trash;



ofstream file;
ofstream file2;
ifstream file3;

void input(){

	file3.open("./input.txt");
	file3 >> trash;
	file3 >> VVcp;
	file3 >> trash;
	file3 >> N_y;
	file3 >> trash;
	file3 >> N_x;
	file3 >> trash;
	file3 >> R_Max;
	file3 >> trash;
	file3 >> trash;
	file3 >> ilosc_cykli;
	file3 >> trash;
	file3.close();
	L_x = N_x*sqrt(VVcp);
	L_y = N_y*sqrt(3*VVcp)*0.5;
	N = N_x * N_y;
}



bool ifOverlaped(double a , double b ,  int n) {

	double distance = 0;
	double distance_x = 0;
	double distance_y = 0;



	for (int i = 0; i < N; i++) {

		if (i == n) {
			continue;
		}
		distance_x = x[i] - a;
		distance_x = sqrt(pow(distance_x , 2));
		distance_y = y[i] - b;
		distance_y = sqrt(pow(distance_y , 2));

		distance_x = min(distance_x , L_x - distance_x);
		distance_y = min(distance_y , L_y - distance_y);



		distance = sqrt(pow(distance_x , 2) + pow(distance_y, 2));


		if (distance < (sigma[n] + sigma[i])*0.5 )  {
			
			return true;
		}
		
	}
	return false;
}

void tables() {

	x = new double[N];
	y = new double[N];
	sigma = new double[N];


}


void MC_step(int n) {
	double a = 0;
	double b = 0;
	double new_x = 0;
	double new_y = 0;

		a = ((double)rand() / (double)(RAND_MAX));
		a = a*(2.0*R_Max) - R_Max;
		b = ((double)rand() / (double)(RAND_MAX));
		b = b*(2.0*R_Max) - R_Max;

		new_x = a + x[n];
		new_y = b + y[n];
		if (new_x > L_x*0.5){
			new_x -= L_x;
		}
		if (new_x < -L_x*0.5){
			new_x += L_x;
		}	
		if (new_y > L_y*0.5){
			new_y -= L_y;
		}
		if (new_y < -L_y*0.5){
			new_y += L_y;
		}	
		
		if(ifOverlaped(new_x, new_y, n) == false){

			x[n] = new_x;
			y[n] = new_y;
			accepted ++;
			all_accepted ++;
		}
		
		

}
void MC_cycle(){
	
	for (int i = 0 ; i < N ; i++){
		
		MC_step(i);
	}
}

void initial_positions(){
	double sum_x = 0;
	double sum_y = 0;
	int index = 0;
  	triangle_network(x , y , sigma , N_x , N_y );
	for (int i = 0 ; i < N ; i++){
		sum_x += x[i];
		sum_y += y[i];

	}
	sum_x = sum_x/N;
	sum_y = sum_y/N;

	for (int i = 0 ; i < N ; i++){
		x[i] -= sum_x;
		y[i] -= sum_y;

	}
}

void wypisz(int number){
	file.open("./data/data" + to_string(number) + ".txt");


	for (int i = 0 ; i < N ; i++){
		
		file << x[i] << " " << y[i] << " " << sigma[i]*0.5 <<endl;
	}
	file.close();
}
int main(){
	srand(time(NULL));
	input();
	tables();
	initial_positions();
	wypisz(0);
	file2.open("./ratio.txt");	
	for (int j = 0 ; j < ilosc_cykli/1000 ; j++){
		for (int i = 0 ; i < 1000 ; i++){
		
			MC_cycle();		
		}
		acceptance_ratio = 100.0*accepted/(1000*N);
		cout << j*1000 << " " << R_Max << " " << acceptance_ratio << endl;
		
		file2 << R_Max << " " << acceptance_ratio << endl;

		if (acceptance_ratio > 25){
			R_Max *= (1.1 + (acceptance_ratio - 25)*0.008);

		}
		else{
			R_Max *= (0.9 - (25 - acceptance_ratio)*0.008);
		}
	
		wypisz(j+1);
		accepted = 0;
	}
	cout << "Average acceptance ratio : " << all_accepted/(ilosc_cykli*N) << endl;
	file2.close();

	return 0;
}
