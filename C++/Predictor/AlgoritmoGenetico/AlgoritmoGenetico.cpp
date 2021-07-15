#include "AlgoritmoGenetico.h"
#include <vector>
#include <algorithm>
#include <math.h>
#include "util.h"
#include "MotorInferencia.h"
#include "Regla.h"
#include "pool.h"

using namespace std;

//PUBLICOS -----------------------------------------------------------------------------------
AlgoritmoGenetico::AlgoritmoGenetico(int nroIndividuos, double mutacion, double seleccion, int minContador, int generaciones, int nroReglas, vector<vector<double>> datos)
{
	this->nroReglas = nroReglas;
	this->nroIndividuos = nroIndividuos;
	this->porcentajeMutacion = mutacion;
	this->porcentajeSeleccion = seleccion;
	this->minCotandor = minContador;
	this->nroGeneraciones = generaciones;
	this->datos = datos;
}

vector<Regla> AlgoritmoGenetico::ejecutar()
{
	poblacionInicial();
	for (int iteracion = 0; iteracion < nroGeneraciones; iteracion++)
	{
		//Evaluamos todos los inviduos
		evaluarPoblacion();

		//Ordenamos los individuos segun fitness
		sort(poblacion.begin(), poblacion.end(), [](const Individuo& left, const Individuo& right) {
			return left.fitness > right.fitness;
			});

		cout << "Iteracion " << iteracion << endl;
		cout << "Fitness " << fMejoresR << endl;
		cout << endl << endl;



		if (iteracion != nroGeneraciones - 1)
		{
			seleccion();
		}
	}
	return mejoresReglas;
}

//PRIVADOS ------------------------------------------------------------------------------------
void AlgoritmoGenetico::poblacionInicial()
{
	//Obtener Dominios Min y Max, Se le saca uno por que se asume que el ultimo es el target
	for (int j = 0; j < datos[0].size() - 1; j++)
	{
		dominioMax.push_back(datos[0][j]);
		dominioMin.push_back(datos[0][j]);
		for (int i = 0; i < datos.size(); i++)
		{
			if (datos[i][j] < dominioMin[j])
			{
				dominioMin[j] = datos[i][j];
			}

			if (datos[i][j] > dominioMax[j])
			{
				dominioMax[j] = datos[i][j];
			}
		}
	}

	//Crear Individuos
	for (int i = 0; i < nroIndividuos; i++)
	{
		vector<double> cromosoma;

		//Medias
		for (int j = 0; j < dominioMax.size(); j++)
		{
			cromosoma.push_back(random_d(dominioMin[j], dominioMax[j]));
		}

		//Desviaciones Estandar
		for (int j = 0; j < dominioMax.size(); j++)
		{
			cromosoma.push_back(random_d(0.3, 0.6) * (dominioMax[j] - dominioMin[j]));
		}

		//Parametros_Pesos
		for (int j = 0; j < dominioMax.size() + 1; j++)
		{
			cromosoma.push_back(random_d(0, 1));
		}

		Individuo ind(cromosoma);
		poblacion.push_back(ind);
	}
}

void AlgoritmoGenetico::seleccion()
{
	vector<Individuo> new_generation;

	//Elitismo: escogemos 50% de mejores Individuos para la siguiente generacion
	int s = rint(this->porcentajeSeleccion * nroIndividuos);
	for (int i = 0; i < s; i++)
		new_generation.push_back(poblacion[i]);

	// El 50% restante se origiara cruzando de manera aleatora
	//los individuos perteneciente al 50% mejor
	s = rint((1 - porcentajeSeleccion) * nroIndividuos);
	for (int i = 0; i < s; i++)
	{
		int r1 = random_d(0, nroIndividuos / 2);
		Individuo padre1 = poblacion[r1];
		int r2 = random_d(0, nroIndividuos / 2);
		while (r1 == r2)
		{
			r2 = random_d(0, nroIndividuos / 2);
		}
		Individuo padre2 = poblacion[r2];
		Individuo hijo = padre1.cruzamiento(padre2, porcentajeMutacion, dominioMin, dominioMax);
		new_generation.push_back(hijo);
	}

	this->poblacion = new_generation;
}

void AlgoritmoGenetico::evaluarPoblacion()
{
	for (int i = 0; i < this->poblacion.size(); i++)
	{
		poblacion[i].contador = 0;
		poblacion[i].fitness = 0;
	}

	vector<vector<Regla>> motores;
	vector<vector<int>> participantes;
	for (int i = 0; i < poblacion.size(); i++)
	{
		while (poblacion[i].contador < minCotandor)
		{
			vector<int> indices;
			indices.push_back(i);
			int index;
			for (int j = 1; j < this->nroReglas; j++)
			{
				index = rand() % poblacion.size();
				while (find(indices.begin(), indices.end(), index) != indices.end())
				{
					index = rand() % poblacion.size();
				}
				indices.push_back(index);
				poblacion[index].contador++;
			}
			poblacion[i].contador++;

			vector<Regla> reglas;
			for (int j = 0; j < indices.size(); j++)
			{
				int nroAtributos = dominioMin.size();
				vector<double> m = slice(poblacion[indices[j]].cromosoma, 0, nroAtributos - 1);
				vector<double> s = slice(poblacion[indices[j]].cromosoma, nroAtributos, nroAtributos * 2 - 1);
				vector<double> w = slice(poblacion[indices[j]].cromosoma, nroAtributos * 2, nroAtributos * 3);
				Regla r(m, s, w);
				reglas.push_back(r);
			}
			motores.push_back(reglas);
			participantes.push_back(indices);
		}
	}

	vector<double> resultados(motores.size());
	ThreadPool pool;
	pool.map<
		function<double(vector<Regla>, vector<vector<double>>*, int, double*)> //Template de la funcion
		, double//Tipo de dato del vector retorno
	>(funcionFitness, motores, &datos, dominioMax.size(), &resultados);


	for (int i = 0; i < participantes.size(); i++)
	{
		double valor = resultados[i];
		if (fMejoresR < valor)
		{
			mejoresReglas = motores[i];
			fMejoresR = valor;
		}
		for (int indice : participantes[i])
		{
			poblacion[indice].fitness += (valor / nroReglas);
		}
	}

	for (int i = 0; i < poblacion.size(); i++)
	{
		poblacion[i].fitness /= poblacion[i].contador;
	}
}

double funcionFitness(vector<Regla> reglas, vector<vector<double>>* datos, int nroAtributos, double* casilla)
{
	MotorInferencia motor(reglas);
	vector<double> predicciones;
	for (int i = 0; i < (*datos).size(); i++)
	{
		vector<double> input = slice((*datos)[i], 0, nroAtributos - 1);
		predicciones.push_back(motor.inferencia(input));
	}

	double fitness = 0;
	for (int i = 0; i < (*datos).size(); i++)
	{
		fitness += pow(((*datos)[i][(*datos)[0].size() - 1] - predicciones[i]), 2);
	}
	fitness = sqrt(fitness / (*datos).size());
	fitness = 1 / (1 + fitness);

	*casilla = fitness;

	return fitness;
}