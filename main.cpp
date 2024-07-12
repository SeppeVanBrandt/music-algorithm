#define _USE_MATH_DEFINES

#include "X.h"
#include "MUSIC.h"
#include <iostream>
#include <iomanip>
#include <fstream>
#include <string>
#include <random>
#include <armadillo>
#include <complex>
#include <cmath>
#include <typeinfo>
#include <math.h>

using namespace std;
using namespace arma;

int main(){

	ifstream file("parameters.txt");
	vector<double> params;
	if (file.good()) {
    double current_number = 0;
    while (file >> current_number){
    	params.push_back(current_number);
    }
    file.close();
	}

	vector<cx_colvec> x_lijst = x((int) params[0],(int) params[1],(int) params[2],(int) params[3],(int) params[4], params[5], vector<double> {params[6],params[7],params[8]}, params[9], vector<double> {params[10],params[11]});
	vector<double> hoeken_lijst;
	for(double h = -90.0; h<90;h=h+0.01){
		hoeken_lijst.push_back(h);
	}
	ofstream spectrum_file;
  spectrum_file.open ("MUSIC_spectrum.txt");
	vector<double> schat_lijst = Estimation(x_lijst, (int) params[3], (int) params[4], 512, params[12], params[5], hoeken_lijst);
	for(int r = 0; r < hoeken_lijst.size(); r++){
		cout << hoeken_lijst[r] << "\t " << schat_lijst[r] << endl;
		spectrum_file << hoeken_lijst[r] << "\t " << schat_lijst[r] << endl;
	}
	spectrum_file.close();
	return 0;
}
