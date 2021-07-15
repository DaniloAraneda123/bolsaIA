#include "cluster.h"
#include <vector>
#include <algorithm>
#include <math.h>
#include <nlohmann/json.hpp>
#include <typeinfo>
#include <string>

using namespace std;

vector<double> calcularVariables(nlohmann::json datos, nlohmann::json centroides)
{
	vector<double> variablesPsicologicas;

    int numero = datos["numero"];

    vector<vector<double>> similitud;

    for (int i=0;i<numero;i++)
    {
        similitud.push_back(vector<double>());
        similitud[i].push_back(cosine_similarity(datos[to_string(i)]["embed"], centroides["politica"]));
        similitud[i].push_back(cosine_similarity(datos[to_string(i)]["embed"], centroides["exterior"]));
        similitud[i].push_back(cosine_similarity(datos[to_string(i)]["embed"], centroides["ecologia"]));
        similitud[i].push_back(cosine_similarity(datos[to_string(i)]["embed"], centroides["economia"]));
        similitud[i].push_back(cosine_similarity(datos[to_string(i)]["embed"], centroides["social"]));
    }

    for (int j=0;j<5;j++)
    {
     
        double valorP = 0, valorN = 0;
        int contP = 0, contN = 0;
        double asd = 0;
        for (int i=0;i<numero;i++)
        {   
            if (j == 0)
            {
                cout << "--------------------------------" << endl;
                cout <<"Tweet " << datos[to_string(i)]["texto"] << endl;
                cout << "Sentimiento " << datos[to_string(i)]["sentimiento"] << endl;
                cout << "Score " << datos[to_string(i)]["score"] << endl;
                cout << "Impacto " << datos[to_string(i)]["impacto"] << endl;
                cout << "Similitud " << similitud[i][j] << endl<<endl;
            }
            if(datos[to_string(i)]["sentimiento"]==1)
            {
                valorP = valorP+((double)datos[to_string(i)]["impacto"] * similitud[i][j]);
                contP++;
                //cout << datos[to_string(i)]["impacto"] << " * " << similitud[i][j] <<" = "<< valorP << " ---- " << contP << endl;
            }
            else 
            {
                valorN = valorN + ((double)datos[to_string(i)]["impacto"] * similitud[i][j]);
                contN++; 
               //cout << datos[to_string(i)]["impacto"] << " * " << similitud[i][j] << " = " << valorN <<" ---- "<< contN<<endl;
            }
        }
        valorP /= contP;
        valorN /= contN;
        if (contP !=0 && contN !=0)
        {
            variablesPsicologicas.push_back(valorP - valorN);
        }
        if (contP == 0)
        {
            variablesPsicologicas.push_back(valorN);
        }
        else if (contN == 0)
        {
                variablesPsicologicas.push_back(valorP);
        }
    }

	return variablesPsicologicas;
}

double cosine_similarity(vector<double> A, vector<double> B)
{
    double dot = 0.0, denom_a = 0.0, denom_b = 0.0;
    for (unsigned int i = 0; i < A.size(); ++i) 
    {
        dot += A[i] * B[i];
        denom_a += A[i] * A[i];
        denom_b += B[i] * B[i];
    }
    return (dot / (sqrt(denom_a) * sqrt(denom_b)) +1)/2;
}