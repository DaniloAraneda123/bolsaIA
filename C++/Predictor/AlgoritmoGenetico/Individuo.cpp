#include <math.h>
#include "Individuo.h"
#include <vector>
#include <random>
#include "util.h"

using namespace std;

Individuo::Individuo(vector<double> cromosoma)
{
	this->cromosoma = cromosoma;
	this->fitness = 0;
	this->contador = 0;
}


Individuo Individuo::cruzamiento(Individuo pareja, double porcentajeMutacion, vector<double> min, vector<double> max)
{
	vector<double> hijo;
	double segmento = (1 - porcentajeMutacion) / 2;
	int nroAtributos = (this->cromosoma.size() - 1) / 3;

	int len = cromosoma.size();
	for (int i = 0; i < len; i++)
	{
		double p = random_d(0, 1);
		if (p < segmento) //Gen de la pareja 1
			hijo.push_back(cromosoma[i]);
		else if (p < segmento * 2)//Gen de la pareja 2
			hijo.push_back(pareja.cromosoma[i]);
		else if (i < nroAtributos)//Mutacion de medias
			hijo.push_back(random_d(min[i], max[i]));
		else if (i >= nroAtributos && i < nroAtributos * 2)//Mutacion de stds
			hijo.push_back(random_d(0.3, 0.6) * (max[i - nroAtributos] - min[i - nroAtributos]));
		else if (i >= nroAtributos * 2)//Mutacion de pesos
			hijo.push_back(random_d(-1, 1));
	}
	return Individuo(hijo);
}
