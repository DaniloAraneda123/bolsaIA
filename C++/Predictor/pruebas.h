#pragma once
#include <iostream>
#include "./AlgoritmoGenetico/Individuo.h"
#include "util.h"
#include "./AlgoritmoGenetico/AlgoritmoGenetico.h"
#include "Presentacion.h" 
#include "MotorInferencia.h"
#include <map>
#include <numeric>

void pruebaMotorInferencia()
{
	vector<double> m({ 7.5, 11 });
	vector<double> s({ 3, 2 });
	vector<double> w({ 4, 0.5, 4 });

	Regla r1(m, s, w);
	double valor = r1.evaluarRegla(vector<double>({ 5, 10 }));
	cout << "Valor de la Regla 1: " << valor << endl;

	double acert = r1.asertividad(vector<double>({ 5, 10 }));
	cout << "Grado Asertividad 1: " << acert << endl << endl << "--------------" << endl;


	m.clear(); s.clear(); w.clear();
	m = { 6.7,9 };
	s = { 1.5,1 };
	w = { 7,4,0.3 };

	Regla r2(m, s, w);
	valor = r2.evaluarRegla(vector<double>({ 5, 10 }));
	cout << "Valor de la Regla 2: " << valor << endl;

	acert = r2.asertividad(vector<double>({ 5, 10 }));
	cout << "Grado Asertividad 2: " << acert << endl << endl << "--------------" << endl;

	MotorInferencia M;
	M.agregarRegla(r1);
	M.agregarRegla(r2);

	double resultado = M.inferencia(vector<double>({ 5, 10 }));

	cout << "El valor predicho es: " << resultado;

}


void probarMutacion()
{
	vector<double> cromosoma1 = { 1,2,34,5,3,2,1, 1,2,34,5,3,2,1 };
	vector<double> cromosoma2 = { 4,3,7,6,45,7,5,4,3,7,6,45,7,5 };

	vector<double> min = { 0,1,0,4,2,1,0, 0,1,0,4,2,1,0 };
	vector<double> max = { 7,4,35,7,46,8,6 ,7,4,35,7,46,8,6 };

	double mutacion = 0.15;

	Individuo padre1(cromosoma1);
	Individuo padre2(cromosoma2);

	printVector(padre1.cromosoma);
	printVector(padre2.cromosoma);

	Individuo hijo = padre1.cruzamiento(padre2, mutacion, min, max);

	printVector(hijo.cromosoma);
}



void probarPoblacionInicial()
{
	vector<vector<double>> matriz;
	for (int i = 0; i < 1000; i++)
	{
		matriz.push_back(vector<double>{});
		for (int j = 0; j < 3; j++)
		{
			double num = random_d(0, 100);
			matriz[i].push_back(num);
		}
	}


	matriz[542][0] = -4;
	matriz[888][0] = 102;

	matriz[2][2] = -1;
	matriz[888][2] = 110;

	matriz[542][1] = 102;
	matriz[888][1] = -10;

	AlgoritmoGenetico a(10, 0.1, 0.5, 100, 100, 30, matriz);
	a.poblacionInicial();

	vector<double> max = a.dominioMax, min = a.dominioMin;
	cout << "Maximos: ";
	printVector(max);
	cout << "Minimos: ";
	printVector(min);

	vector<Individuo> pobla = a.poblacion;

	cout << "Largo Cromosoma generado: " << pobla[0].cromosoma.size() << endl << endl;

	for (int i = 0; i < pobla.size(); i++)
	{
		cout << "Nro " << i << ":    ";
		printVector(pobla[i].cromosoma);
	}
}

void probarSeleccion()
{
	vector<vector<double>> matriz;
	for (int i = 0; i < 1000; i++)
	{
		matriz.push_back(vector<double>{});
		for (int j = 0; j < 3; j++)
		{
			double num = random_d(0, 100);
			matriz[i].push_back(num);
		}
	}

	AlgoritmoGenetico a(10, 0.1, 0.5, 100, 100, 20, matriz);
	a.poblacionInicial();

	for (int i = 0; i < a.poblacion.size(); i++) {
		a.poblacion[i].fitness = random_d(0, 100);
	}


	sort(a.poblacion.begin(), a.poblacion.end(), [](const Individuo& left, const Individuo& right) {
		return left.fitness > right.fitness;
		});

	cout << "Antes de Seleccion" << endl;
	for (int i = 0; i < a.poblacion.size(); i++)
	{
		cout << "Nro :" << i << "---" << a.poblacion[i].fitness << "   ";
		printVector(a.poblacion[i].cromosoma);
	}

	cout << endl << endl;

	a.seleccion();

	cout << "Despues de Seleccion" << endl;
	for (int i = 0; i < a.poblacion.size(); i++)
	{
		cout << "Nro :" << i << "---" << a.poblacion[i].fitness << "   ";
		printVector(a.poblacion[i].cromosoma);
	}
}

void probarPresentacion()
{
	Presentacion presentacion;
	vector<vector<double>> matriz;
	matriz = presentacion.leerDatos("amd1.csv");


	for
		(int i = 0; i < matriz.size(); i++)
	{
		for (int j = 0; j < matriz[0].size(); j++)
		{
			cout << matriz[i][j] << "\t";
		}

		cout << "\n";

	}

}

void probarGenetico()
{
	vector<vector<double>> matriz;
	for (int i = 0; i < 50; i++)
	{
		matriz.push_back(vector<double>{});
		for (int j = 0; j < 4; j++)
		{
			double num = random_d(0, 100);
			matriz[i].push_back(num);
		}
	}
	vector<double> input = matriz.back();
	matriz.pop_back();
	input.pop_back();

	AlgoritmoGenetico a(60, 0.05, 0.5, 30, 20, 30, matriz);
	vector<Regla> r = a.ejecutar();
	MotorInferencia m(r);
	double valor = m.inferencia(input);
	cout << endl << endl << endl << "El valor predicho es :" << valor;
}