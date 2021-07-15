#pragma once

#include <random>
#include <ctime>
#include <iostream>
#include <vector>
#include "Regla.h"

double random_d(double min, double max);

template<typename T>
vector<T> slice(vector<T> const& v, int m, int n)
{
	auto first = v.cbegin() + m;
	auto last = v.cbegin() + n + 1;

	vector<T> vec(first, last);
	return vec;
}

template<typename T>
void printVector(std::vector<T> const& v)
{
	for (auto i : v) {
		std::cout << i << ' ';
	}
	std::cout << '\n';
}

double funcionFitness(vector<Regla> reglas, vector<vector<double>>* datos, int nroAtributos, double* casilla);