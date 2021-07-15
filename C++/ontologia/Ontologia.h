#pragma once

#include <string>
#include <iostream>
#include <fstream>
#include <map>
#include <vector>
#include <nlohmann/json.hpp>




using json = nlohmann::json;



using namespace std;

class Ontologia
{

public:

	vector<string> objetos;
	vector<string> atributos;
	string result_type;
	string fecha;
	int max_items;

	Ontologia();
	Ontologia(string accion, string fecha);
	vector<string>leerUsuarios();
	vector<string>leerKeywords();
	void generarObjetos();
	void generarAtributos();
	json getQuery();
};


