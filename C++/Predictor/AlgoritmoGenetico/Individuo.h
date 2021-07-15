#pragma once
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>

using namespace std;

class Individuo
{
public:
	vector<double> cromosoma;
	double fitness;
	int contador;

	Individuo(vector<double> cromosoma);

	Individuo cruzamiento(Individuo pareja, double porcentajeMutacion, vector<double> min, vector<double> max);
};