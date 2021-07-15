#include "MotorInferencia.h"
#include <string>
#include <vector>
#include "Regla.h"
#include <numeric>

using namespace std;

vector<double> MotorInferencia::calcularPonderacion(vector<double> inputs)
{
	vector<double> ponderaciones;
	double pond = 0;

	for (int i = 0; i < reglas.size(); i++)
	{
		pond = reglas[i].asertividad(inputs);
		ponderaciones.push_back(pond);
	}

	return ponderaciones;
}

vector<double> MotorInferencia::evaluarReglas(vector<double> inputs)
{
	vector<double> valores;
	double valor = 0;
	for (int i = 0; i < reglas.size(); i++)
	{
		valor = reglas[i].evaluarRegla(inputs);
		valores.push_back(valor);
	}
	return valores;
}

MotorInferencia::MotorInferencia(vector<Regla> reglas)
{
	this->reglas = reglas;
}

MotorInferencia::MotorInferencia()
{
}

void MotorInferencia::agregarRegla(Regla r)
{
	reglas.push_back(r);
}

double MotorInferencia::inferencia(vector<double> inputs)
{
	double resultado = 0;
	vector<double> valorRegla = evaluarReglas(inputs);
	vector<double> pondRegla = calcularPonderacion(inputs);

	for (int i = 0; i < reglas.size(); i++)
	{
		resultado = resultado + (valorRegla[i] * pondRegla[i]);
	}
	resultado = resultado / accumulate(pondRegla.begin(), pondRegla.end(), 0.0);

	return resultado;
}