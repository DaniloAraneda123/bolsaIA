#pragma once

#include <string>
#include <iostream>
#include <fstream>
#include <map>
#include <vector>
#include "MotorInferencia.h"
#include "Regla.h"

using namespace std;

class Presentacion
{
private:

    bool sesion;

public:

    Presentacion();
    double predecirPrecioAccion(string nombre_accion);
    map<string, double> predecirPrecioAccionCompleto(string nombre_accion);
    vector<vector<double>> leerDatos(string nombre_archivo);
    vector<double> leerInput(string nombre_archivo);

    void generarReporte(map<string, double> resultados, string nombre_archivo);
    void generarBaseConocimiento(string opcion, vector<Regla> r);
    void generarBaseConocimiento1(string opcion, vector<Regla> r);
    void mostrarResultadoPantalla(double precio_accion);
    void mostrarResultadoPantallaCompleto(map<string, double> resultados, string nombre_archivo);

    bool getSesion();
    void setSesion(bool sesion);

};