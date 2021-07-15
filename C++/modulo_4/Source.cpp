#include <iostream>
#include <nlohmann/json.hpp>
#include <cpr/cpr.h>
#include <string>
#include <vector>
#include "cluster.h"


int main()
{
	cpr::Response r1 = cpr::Get(cpr::Url{ "http://localhost:5000/endpoint1" });
	nlohmann::json datos = nlohmann::json::parse(r1.text);

	cpr::Response r2 = cpr::Get(cpr::Url{ "http://localhost:5000/endpoint2" });
	nlohmann::json centroides = nlohmann::json::parse(r2.text);

	vector<double> resultados = calcularVariables(datos, centroides);

	return 0;
}