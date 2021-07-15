#pragma once
#include <string>
#include <vector>
#include "Regla.h"

using namespace std;

class MotorInferencia
{
private:
	vector<Regla> reglas;
	int nroAtributos;

	vector<double> calcularPonderacion(vector<double> inputs);

	vector<double> evaluarReglas(vector<double> inputs);

public:
	MotorInferencia();

	MotorInferencia(vector<Regla> reglas);

	void agregarRegla(Regla r);

	double inferencia(vector<double> inputs);
};