#include "Presentacion.h"
#include "MotorInferencia.h"
#include "Presentacion.h"
#include <string>
#include <fstream>
#include <vector>
#include <map>
#include <set>
#include <string>
#include <utility> 
#include <stdexcept> 
#include <sstream> 
#include <iostream>
#include <iterator> 
#include "cluster.h"
#include "Ontologia.h"
#include <cpr/cpr.h>

using namespace std;


 Presentacion::Presentacion(){

     sesion = true;
}

 bool Presentacion::getSesion() {

	 return sesion;

 }

 void Presentacion::setSesion(bool _sesion) {

	 this->sesion = _sesion;

 }


 double Presentacion::predecirPrecioAccion(string nombre_accion) {

	 vector<double> inputs(2);
	 vector<vector<double>> base_conocimiento = leerDatos("base_conocimiento.csv");
	 vector<double> m;
	 vector<double> s;
	 vector<double> w;
	 MotorInferencia M;
	 double atributos = (base_conocimiento[0].size() - 1) / 3;
	 for (int i = 0; i < base_conocimiento.size(); i++)
	 {
		 for (int j = 0; j < base_conocimiento[0].size(); j++)
		 {
			 if (j < atributos) {
				 m.push_back(base_conocimiento[i][j]);

			 }
			 else if (j < 2 * atributos) {

				 s.push_back(base_conocimiento[i][j]);
			 }
			 else {

				 w.push_back(base_conocimiento[i][j]);
			 }

		 }
		 M.agregarRegla(Regla(m, s, w));
		 m.clear();
		 s.clear();
		 w.clear();
	 }




	 if (nombre_accion == "AMD") {

		 inputs[0] = 5;
		 inputs[1] = 10;
		 // inputs[2] = 90.83;
		 //inputs[3] = 91.77;
		 //inputs[4] = 60712900;
		 //inputs[5] = 23.07;
		 //inputs[6] = 24.18;
		 //inputs[7] = 21.92;
		 //inputs[8] = 22.11;

	 }
	 double precio_accion = M.inferencia(inputs);

	 return precio_accion;



 }

 map<string, double> Presentacion::predecirPrecioAccionCompleto(string nombre_accion) 
 {
	 cout << "Leemos las Reglas creadas" << endl;
	 vector<vector<double>> base_conocimiento = leerDatos("baseConocimiento.csv");
	 vector<double> m;
	 vector<double> s;
	 vector<double> w;
	 MotorInferencia M;
	 double atributos = (base_conocimiento[0].size() - 1) / 3;
	 for (int i = 0; i < base_conocimiento.size(); i++)
	 {
		 for (int j = 0; j < base_conocimiento[0].size(); j++)
		 {
			 if (j < atributos) {
				 m.push_back(base_conocimiento[i][j]);

			 }
			 else if (j < 2 * atributos) {

				 s.push_back(base_conocimiento[i][j]);
			 }
			 else {

				 w.push_back(base_conocimiento[i][j]);
			 }

		 }
		 M.agregarRegla(Regla(m, s, w));
		 m.clear();
		 s.clear();
		 w.clear();
	 }

	 cout << "------------------------------------------------------------------------------" << endl;
	 system("pause");
	 system("cls");

	 // Leer input
	 vector<double> inputs;
	 inputs = leerInput(nombre_accion);

	 double precio_accion = M.inferencia(inputs);
	 map<string, double> aux;

	 
	 aux.insert({ "Precio Accion($Dolar)", precio_accion+ 3699.2325544 });
	 //aux.insert({ "Variacion Porcentual (%)", 100 * (precio_accion - inputs[3]) / inputs[3] });

	 cout <<endl<<endl<<"El precio de la accion predicho para el dia 15/07: "<< precio_accion + 3699.2325544<<endl<<endl<<
		 "-------------------------------------------------------------------------------------------------";

	 return aux;
 }


 vector<double> Presentacion::leerInput(string nombre_archivo) {

	 string linea, campo;
	 stringstream ss;
	 vector<double> inputs;



	 ifstream lectura;
	 lectura.open("input.csv", ios::in);

	 if (!lectura) {

		 cout << "No hay archivo con ese nombre";
		 exit(1);
	 }

	 system("CLS");
	 cout <<"Los inputs son:"<<endl;
	 for (string linea; getline(lectura, linea); )
	 {

		 stringstream registro(linea);

			 
		 for (string dato; getline(registro, dato, ','); )
		 {
					inputs.push_back(stod(dato));
					cout << " | " << dato;
		  }
	
	}
	 return inputs;

 }
 vector<vector<double>> Presentacion::leerDatos(string nombre_archivo) {

	
	string linea, campo;
	stringstream ss;
	vector<vector<double>> datos;
	vector<double> fila;
	int columna = 0;
	int i = 0;


	ifstream lectura;
	lectura.open(nombre_archivo, ios::in);

	if (!lectura) {

		cout << "No ha archivo con ese nombre";
		exit(1);
	}

	for (string linea; getline(lectura, linea); )
	{

		if (i > 0) {

		
			stringstream registro(linea);

			columna = 0;
			for (string dato; getline(registro, dato, ','); )
			{
				if (columna > 0) {
					cout << dato << '\t';
					fila.push_back(stod(dato));
				}
				columna += 1;
				//j += 1;
				
			}
			datos.push_back(fila);
			fila.clear();
		}
		cout << '\n';

		
		i += 1;
	}

	return datos;

}

 void Presentacion::generarReporte(map<string, double> resultados, string nombre_archivo) {


		 std::ofstream myfile;

		 string nombre = "Reportes/Reporte_" + nombre_archivo + ".csv";
		 //cout << nombre << "ruta";
		 myfile.open(nombre);
		 
		 myfile << "Reporte sobre la prediccion realizada para obtener el precio de la accion " + nombre_archivo + " el dia \n";
		 myfile << " \n \n ";
		 map <string, double> ::iterator it;
		 int contador = 0;
		 for (it = resultados.begin(); it != resultados.end(); it++) {

			 myfile << it->first;
			 myfile << "    :    ";
			 myfile << it->second;


			 myfile << " \n \n ";
			 //cout << it->first << "it";

		 }

		 myfile << " \n \n ";

		 int limite = 1;
		 if (resultados["j) Variacion Porcentual (%)"] > limite) {
			 myfile << " ******************* Es ALTAMENTE recomendable COMPRAR hoy acciones de " + nombre_archivo + " **********************************";
		 }
		 if (resultados["j) Variacion Porcentual (%)"] > 0 && resultados["j) Variacion Porcentual (%)"] <= limite) {

			 myfile << " *******************Es recomendable COMPRAR hoy acciones de " + nombre_archivo  + " **********************************";

		 }
		 if (resultados["j) Variacion Porcentual (%)"] < 0 && resultados["j) Variacion Porcentual (%)"] >= -limite) {

			 myfile << "******************* Es recomendable VENDER hoy acciones de " + nombre_archivo + " **********************************";

		 }
		 if (resultados["j) Variacion Porcentual (%)"] < -limite) {

			 myfile << "******************* Es ALTAMENTE recomendable VENDER hoy acciones de " + nombre_archivo  + " **********************************";

		 }

		 myfile << "\nVariables: \n";
		 myfile << "a) Precio inicial en el dia anterior a la prediccion\n";
		 myfile << "b) Maximo precio cotizado el dia anterior a la prediccion\n";
		 myfile << "c) Minimo precio cotizado el dia anterior a la prediccion\n";
		 myfile << "d) Precio de cierre en el dia anterior a la prediccion\n";
		 myfile << "e) Valor del indice VIX de apertura en el dia anterior a la prediccion\n";
		 myfile << "f) Maximo valor del indice VIX en el dia anterior a la prediccion\n";
		 myfile << "g) Minimo valor del indice VIX en el dia anterior a la prediccion\n";
		 myfile << "h) Valor del indice VIX de cierre el dia anterior a la prediccion\n";
		 myfile << "\n";
		 myfile << "VIX corresponde al valor de las expectitativas de la volatidad del mercado para los proximos 30 dias. Indice psicologico";
		 myfile.close();

 }

 void Presentacion::generarBaseConocimiento(string opcion, vector<Regla> r) {

	 std::ofstream myfile;
	 char delimitador = ',';

	 string nombre = "baseConocimiento.csv";
	 myfile.open(nombre);
	 myfile << "numeroRegla";
	 for (int i = 0; i < r[0].getMedias().size(); i++) {
		 myfile << delimitador;
		 myfile << "media";
		 myfile << i;
	 }
	 for (int i = 0; i < r[0].getStds().size(); i++) {
		 myfile << delimitador;
		 myfile << "desviacion_estandar";
		 myfile << i;
	 }
	 for (int i = 0; i < r[0].getPesos().size(); i++) {
		 myfile << delimitador;
		 myfile << "pesos";
		 myfile << i;
	 }
	 myfile << "\n";


	 for (int j = 0; j < r.size(); j++) {
		 myfile << j;
		 for (int i = 0; i < r[j].getMedias().size(); i++) {
			 myfile << delimitador;
			 myfile << r[j].getMedias()[i];
		 }
		 for (int i = 0; i < r[j].getStds().size(); i++) {
			 myfile << delimitador;
			 myfile << r[j].getStds()[i];
		 }
		 for (int i = 0; i < r[j].getPesos().size(); i++) {
			 myfile << delimitador;
			 myfile << r[j].getPesos()[i];
		 }
		 myfile << "\n";
	 }
 }

 void Presentacion::generarBaseConocimiento1(string opcion, vector<Regla> r) {

	 std::ofstream myfile;
	 char delimitador = ',';
	 vector<string> atributos = { "x1","x2"};

	 

	 string nombre = "baseConocimientoVisual.csv";
	 myfile.open(nombre);

	 myfile << "Numero de Reglas: " + to_string(r.size()) + "\n";
	 myfile << "Diccionario:\n";
	 myfile << "x1 = Caracteristica_1\n";
	 myfile << "x2 = Caracteristica_2\n";
	 
	 for (int i = 0; i < r.size(); i++) {
		 myfile << "IF ";
		 for (int j = 0; j < 2; j++) {
			 myfile << atributos[j] + " IS " + "N(" + to_string(r[i].getMedias()[j]) + "," + to_string(r[i].getStds()[j]) + ")";
			 if (j < 7) {
				 myfile << " AND ";
			 }
		 }
		 myfile << " THEN ";
		 for (int j = 0; j < 3; j++) {
			 if (j == 0) {
				 myfile << to_string(r[i].getPesos()[j]);
			 }
			 else{
				 myfile << " + " + to_string(r[i].getPesos()[j]) + " " + atributos[j-1];
			}
		 }

		 myfile << "\n\n";
	 }
 }

 void Presentacion::mostrarResultadoPantalla(double precio_accion) {

	 cout << "Precio accion:   " << precio_accion;

 }
 void Presentacion::mostrarResultadoPantallaCompleto(map<string, double> resultados, string nombre_archivo) {




	cout << "\n\nReporte sobre la prediccion realizada para obtener el precio de la accion " + nombre_archivo + " el dia \n";
	 cout  << " \n \n ";
	 map <string, double> ::iterator it;
	 int contador = 0;
	 for (it = resultados.begin(); it != resultados.end(); it++) {

		 cout << it->first;
		 cout  << "    :    ";
		 cout << it->second;


		 cout << " \n \n ";
		 //cout << it->first << "it";

	 }

	 cout << " \n \n ";

	 int limite = 1;
	 if (resultados["j) Variacion Porcentual (%)"] > limite) {
		 cout << "\n *******************Es ALTAMENTE recomendable COMPRAR acciones de " + nombre_archivo + "**********************************";
	 }
	 if (resultados["j) Variacion Porcentual (%)"] > 0 && resultados["j) Variacion Porcentual (%)"] <= limite) {

		 cout << "\n *******************Es recomendable COMPRAR acciones de " + nombre_archivo + " **********************************";

	 }
	 if (resultados["j) Variacion Porcentual (%)"] < 0 && resultados["j) Variacion Porcentual (%)"] >= -limite) {

		 cout << "\n*******************Es recomendable VENDER acciones de " + nombre_archivo + "**********************************";

	 }
	 if (resultados["j) Variacion Porcentual (%)"] < -limite) {

		 cout << "\n*******************Es ALTAMENTE recomendable VENDER acciones de " + nombre_archivo + "**********************************";

	 }

	 cout << "\n\nVariables: \n";
	 cout << "a) Precio inicial en el dia anterior a la prediccion\n";
	 cout << "b) Maximo precio cotizado el dia anterior a la prediccion\n";
	 cout << "c) Minimo precio cotizado el dia anterior a la prediccion\n";
	 cout << "d) Precio de cierre en el dia anterior a la prediccion\n";
	 cout << "e) Valor del indice VIX de apertura en el dia anterior a la prediccion\n";
	 cout << "f) Maximo valor del indice VIX en el dia anterior a la prediccion\n";
	 cout << "g) Minimo valor del indice VIX en el dia anterior a la prediccion\n";
	 cout << "h) Valor del indice VIX de cierre el dia anterior a la prediccion\n";
	 cout << "\n";
	 cout << "VIX corresponde al valor de las expectitativas de la volatidad del mercado para los proximos 30 dias. Indice psicologico";
	 


 }