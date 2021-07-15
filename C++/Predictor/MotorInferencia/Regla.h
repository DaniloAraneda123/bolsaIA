#pragma once

#include <string>
#include <vector>

using namespace std;

class Regla
{
private:
	vector<double> medias;
	vector<double> stds;
	vector<double> pesos;

public:
	Regla(vector<double> m, vector<double> s, vector<double> w);

	double evaluarRegla(vector<double> inputs);

	double asertividad(vector<double> inputs);

	vector<double> getMedias();
	vector<double> getStds();
	vector<double> getPesos();


};