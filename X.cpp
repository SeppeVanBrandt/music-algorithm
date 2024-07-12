cd #define _USE_MATH_DEFINES

//#include "S.h"
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

vector<cx_colvec> S_matrix(int L_low, int L_upp, int T, int K){
        random_device rd;
        default_random_engine generator(rd());
        exponential_distribution<double> exp_distribution;
        uniform_int_distribution<int> int_distribution(0,1);
        uniform_int_distribution<int> L_distribution(L_low, L_upp);
        uniform_real_distribution<double> sigma_distribution(0, 1);
        vector<int> plusofmin = {-1,1};
        vector<vector<cx_double>> s_lijst(K);
        for(int k = 0; k < K; k++){
                int  T_cur = 0;
                while(T_cur < T-L_upp){
                        double L = L_distribution(generator);
                        double sigma = 1; //sigma_distribution(generator);
                        exp_distribution = exponential_distribution<double>(1/sigma);
                        for(int t = T_cur; t < T_cur+L; t++){
                                s_lijst[k].push_back(
                                                cx_double(
                                                        exp_distribution(generator)
                                                        *plusofmin[int_distribution(generator)],
                                                        exp_distribution(generator)
                                                        *plusofmin[int_distribution(generator)]));
                        }
                        T_cur += L;
//                      cout << k << "\t" << T_cur << endl;
                }
                for(int t = T_cur; t < T; t++){
                        s_lijst[k].push_back(
                                        cx_double(
                                                exp_distribution(generator)*plusofmin[int_distribution(generator)],
                                                exp_distribution(generator)*plusofmin[int_distribution(generator)]));
                }
        }

	vector<cx_colvec> ret;
        for(int t = 0; t < T; t++){
		cx_colvec vec_temp(K);
		for(int k = 0; k < K; k++){
			vec_temp.row(k) = s_lijst[k][t];
		}
		ret.push_back(vec_temp);
	}
	for(int t = 0; t < T; t++){
//		cout << t << endl;
//		ret[t].print();
	}
	return ret;
}

cx_mat A(int N, int K, double lambda, vector<double> elementen, vector<double> hoeken){          //elementen: coördinaten op roosteras van elementen
  cout << "test1" << endl;
  cx_mat A_temp(N,K);
  cout << N << K << endl;
	for(int h=0; h<hoeken.size(); h++){
		vector<cx_double> a_lijst;
		for(int n=0; n<elementen.size(); n++){
			a_lijst.push_back(exp(-i*2.0*M_PI*(1/lambda)*elementen[n]*sin(hoeken[h]*(M_PI/180))));
		}
    cout << h << hoeken.size() << endl;
		A_temp.col(h)=conv_to<cx_colvec>::from(a_lijst);
    cout << h << hoeken.size() << endl;
	}
	return A_temp;
}

vector<cx_colvec> v(int T, int N, double noise_sigma){
	random_device rd;
        default_random_engine generator(rd());
	normal_distribution<double> distribution(0,noise_sigma);
	vector<cx_colvec> v_temp;
	for(int t = 0; t < T; t++){
		vector<cx_double> v_lijst;
		for(int i = 0; i < N; i++){
			v_lijst.push_back(distribution(generator));
		}
		v_temp.push_back(conv_to<cx_colvec>::from(v_lijst));
	}
	return v_temp;
}

vector<cx_colvec> x(int L_low, int L_upp, int T, int K, int N, double lambda, vector<double> elementen, double noise_sigma, vector<double> hoeken){

  vector<cx_colvec> s_temp = S_matrix(L_low, L_upp, T, K);
	cx_mat A_temp = A(N, K, lambda, elementen, hoeken);
	vector<cx_colvec> v_temp = v(T, N, noise_sigma);
	vector<cx_colvec> x_temp;
	for(int t = 0; t < T; t++){
		x_temp.push_back(A_temp*s_temp[t] + v_temp[t]);
	}
	return x_temp;
}
