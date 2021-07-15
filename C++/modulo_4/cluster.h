#pragma once

#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <nlohmann/json.hpp>

using namespace std;

vector<double> calcularVariables(nlohmann::json datos, nlohmann::json centroides);

double cosine_similarity(vector<double> A, vector<double> B);