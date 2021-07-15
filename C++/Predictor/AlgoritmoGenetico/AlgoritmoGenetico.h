#pragma once

#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include "Individuo.h"
#include "Regla.h"

using namespace std;

class AlgoritmoGenetico
{
public:
	AlgoritmoGenetico(int nroIndividuos, double mutacion, double seleccion, int minContador, int generaciones, int nroReglas, vector<vector<double>> datos);
	vector<Regla> ejecutar();

	//private:
		//Atributos
	int nroReglas;
	vector<Regla> mejoresReglas;
	double fMejoresR = 0;
	vector<Individuo> poblacion;
	int nroIndividuos;  //H
	double porcentajeSeleccion;  //H
	double porcentajeMutacion;  //H
	int minCotandor;  //H
	int nroGeneraciones; //H
	vector<double> dominioMax;
	vector<double> dominioMin;
	vector<vector<double>> datos;  //H

	//Metodos
	void poblacionInicial();
	void seleccion();
	//double static funcionFitness(vector<Regla> reglas, vector<vector<double>>* datos, int nroAtributos);
	void evaluarPoblacion();
};