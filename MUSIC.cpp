#define _USE_MATH_DEFINES

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

const cx_double i(0.0,1.0);



cx_colvec a(int N, double lambda, double d, double angle){
	vector<cx_double> temp;
	temp.push_back(1.0);
	for(int n = 1; n < N; n++){
		temp.push_back(exp(-i*2.0*M_PI*(d/lambda)*(double)n*sin(angle*(M_PI/180))));
	}
	return conv_to<cx_colvec>::from(temp);
}

vector<double> Estimation(vector<cx_colvec> x, int K, int N, int L, double d, double lambda, vector<double> hoeken_lijst){
	int T = x.size();
	if(T % L != 0){
		throw "T must be divisible by L.";
	}
	int M = T/L;

  cx_mat sum = conv_to<cx_mat>::from(zeros(N,N));
  for(int m =0; m < M; m++){
    for(int t = m*L; t < (m+1)*L-1; t++){
		  sum += x[t]*x[t].t();
	  }
  }
  cx_mat Y = sum;

	cx_mat U;
	vec s;
	cx_mat V;
	svd(U, s, V, Y);
	cx_mat U_n = U.cols(K,N-1);

	vector<double> temp;
	for(int o = 0; o < hoeken_lijst.size();  o++){
		cx_double noemer = cdot(U_n.t()*a(N,lambda,d,hoeken_lijst[o]),U_n.t()*a(N,lambda,d,hoeken_lijst[o]));
		temp.push_back(1.0/abs(noemer));
	}
	return temp;
}
