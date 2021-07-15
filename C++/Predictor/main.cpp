#include <iostream>
#include "Individuo.h"
#include "util.h"
#include "AlgoritmoGenetico.h"
#include "Presentacion.h" 
#include "MotorInferencia.h"
#include <map>
#include <numeric>
#include "pruebas.h"
#include <nlohmann/json.hpp>
#include <cpr/cpr.h>
#include "cluster.h"
#include "Ontologia.h"


using namespace std;

double random_d(double min, double max)
{
	static default_random_engine generator(unsigned(time(nullptr)));
	uniform_real_distribution<double> distribution(min, max);
	return distribution(generator);
}


vector<double> obtenerVariables()
{
	Ontologia o("amazon", "2021-07-14");
	nlohmann::json consulta = o.getQuery(); 

	cout <<"La consulta es: "<< consulta << endl;
	system("CLS");
	cout << "......" << endl;

	cpr::Response r1 = cpr::Post(cpr::Url{"http://localhost:5000/endpoint1" },cpr::Body{ consulta.dump() },cpr::Header{ {"Content-Type", "application/json"} });
	nlohmann::json datos = nlohmann::json::parse(r1.text);

	cpr::Response r2 = cpr::Get(cpr::Url{ "http://localhost:5000/endpoint2" });
	nlohmann::json centroides = nlohmann::json::parse(r2.text);

	vector<double> resultados = calcularVariables(datos, centroides);

	return resultados;
}

void crearBaseConocimiento(string opcion) 
{
	Presentacion presentacion;
	cout << "Leemos los datos de entrenamiento ya normalizados" << endl<<endl;
	cout << "Caracteristica_1" << "\t" << "Caracteristica_2" << "\t" <<"Target"<< endl;
	vector<vector<double>> matriz = presentacion.leerDatos("entrenamiento.csv");
	cout << ".........." << endl;
	cout << endl << "-------------------------------------------------------------------------------" << endl;
	system("pause");
	system("CLS");
	AlgoritmoGenetico a(60, 0.05, 0.5, 20, 80, 20, matriz);
	vector<Regla> r = a.ejecutar();
	string s;
	cout << "\nDesea guardar esta base de conocimiento (s/n): "; // s para indicar que sE cualquier otra letra para indicar no
	cin >> s;
	if (s == "s") {
		presentacion.generarBaseConocimiento1(opcion, r);
		presentacion.generarBaseConocimiento(opcion, r);
	}
	else {
		cout << "\No se guardo";
	}
	system("CLS");
}

void psico_test()
{
	vector<double> v_psicos = obtenerVariables();

	vector<string> labels({ "Politica","Exterior","Ecologia","Economia","Social"});

	system("pause");
	system("CLS");
	cout << "Varible Psicologicas" << endl<<endl;
	for (int i = 0; i < v_psicos.size(); i++)
	{
			cout << labels[i]<<": " << v_psicos[i] <<endl;
	}
	cout << endl;
}

void predecir_test()
{
	map <string, double> resultados;
	Presentacion presentacion;
	string nombre="Amazon";

	crearBaseConocimiento(nombre);

	resultados = presentacion.predecirPrecioAccionCompleto(nombre);
		
}


int main()
{
	psico_test();
	//predecir_test();
	return 0;
}