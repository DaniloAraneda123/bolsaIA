#include <math.h>
#include <vector>
#include <string>
#include "Regla.h"
#include <iostream>
#include "algorithm"

using namespace std;

Regla::Regla(vector<double> m, vector<double> s, vector<double> w)
{
	medias = m;
	stds = s;
	pesos = w;
}

vector<double> Regla::getMedias() {

	return medias;
}

vector<double> Regla::getStds() {

	return stds;
}

vector<double> Regla::getPesos() {

	return pesos;
}


double Regla::evaluarRegla(vector<double> inputs)
{
	double resultado = pesos[0];
	for (int i = 1; i < pesos.size(); i++)
	{
		resultado = resultado + inputs[i - 1] * pesos[i];
	}
	return resultado;
}

double Regla::asertividad(vector<double> inputs)
{
	vector<double> gradoA;
	double arg;
	for (int i = 0; i < medias.size(); i++)
	{
		arg = ((inputs[i] - medias[i]) / stds[i]);
		gradoA.push_back(exp(-(arg * arg) / 2));
	}
	return *min_element(gradoA.begin(), gradoA.end());
}