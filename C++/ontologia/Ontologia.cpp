#include "Ontologia.h"
#include <string>
#include <fstream>
#include <vector>
#include <map>
#include <set>
#include <string>
#include <utility> 
#include <stdexcept> 
#include <sstream> 
#include <nlohmann/json.hpp>



using json = nlohmann::json;
using namespace std;


Ontologia::Ontologia(string accion, string _fecha) {

    string opcion = "-1";
    string usuario;
    string keyword;
    while (opcion != "0" && opcion != "1" && opcion != "2") {

        cout << "Elija opcion de busqueda de tweets:" << endl;
        cout << endl;
        cout << endl;
        cout << "  1) Usuarios" << endl;
        cout << "  2) Keywords" << endl;
        cout << endl;
        cout << "  0) Salir" << endl;
        cout << endl;
        cout << ":";
        cin >> opcion;
        cout << flush;
        system("CLS");

    }
    if (opcion == "1") {

        cout << "USUARIOS " << endl;
        cout << endl;
        cout << "Marque 0) si ha terminado de ingresar usuarios " << endl;
        cout << endl;
        cout << endl;

        while (usuario != "0") {
            cout << "Ingrese usuario de twitter: ";
            cin >> usuario;
            if (usuario != "0") {
                objetos.push_back(usuario);
                //cout << usuario << "  " << "usuario ingresado correctamente!" << endl;

            }
        }

    }
    if (opcion == "2") {
        cout << "KEYWORD " << endl;
        cout << endl;
        cout << "Marque 0) si ha terminado de ingresar keywords " << endl;
        cout << endl;
        cout << endl;

        while (keyword != "0") {

            cout << "Ingrese keyword que considere relevante en la prediccion: ";
            cin >> keyword;
            if (keyword != "0") {
                atributos.push_back(keyword);
                //cout << keyword << "  " << "keyword ingresado correctamente!" << endl;

            }
        }

    }


    result_type = "popular";
    max_items = 10;
    fecha = _fecha;

}

Ontologia::Ontologia() {

    result_type = "recent"; // Puede ser mixed, popular, recent
    max_items = 10; // Aca hay que tener cuidado con el limite de 100 consultas con ventanas de 15 minutos

    // Aca deberia elegir los keywords y usuarios de alguna manera mas inteligente o que el mismo usuario lo elija
    generarAtributos();
    generarObjetos();

    // Guardo los usuarios y keywords
    objetos = leerUsuarios();
    atributos = leerKeywords();
    fecha = "2021-07-15";

}


json Ontologia::getQuery() {


    /*
    for (int i = 0; i < objetos.size(); i++) {
        cout << objetos[i] << "\n";;
    }
    for (int i = 0; i < atributos.size(); i++) {
        cout << atributos[i] << "\n";
    }
    cout << result_type;
    cout << max_items;
    */
    json j;

    j["user"] = objetos;
    j["keyword"] = atributos;
    j["result_type"] = result_type;
    j["max_tweets"] = max_items;
    j["fecha"] = fecha;

    return j;




}

void Ontologia::generarObjetos() {



    vector<string> aux;
    aux.push_back("elonmusk");
    aux.push_back("CNNBusiness");
    aux.push_back("nytimes");
    aux.push_back("elonmusk");
    aux.push_back("WarrenBufett");

    objetos = aux;

    /*
    json = {"user":["elonmusk", "BarackObama"], "keyword":["space", "realize"],"result_type":"recent","max_tweets":"5"}
        @business 50 551
        @Carl_C_Icahn NA * 2
        @CNNBusiness 747 1725
        @ecb NA * 298
        @Investingcom NA * 279
        @InvestOfceAD NA * 120
        @jpmorgan NA * 86
        @JPMorganAM NA * 30
        @lloydblankfein NA * 8
        @nytimes 490 244
        @SEC_Enforcement NA * 2
        @UBS_CEO NA * 9
        @USTreasury NA * 78
        @WarrenBufett
    */

}

void Ontologia::generarAtributos() {

    vector<string> aux;
    aux.push_back("tax");
    aux.push_back("stock");
    aux.push_back("amazon");
    aux.push_back("war");
    aux.push_back("bezos");


    atributos = aux;


}

vector<string> Ontologia::leerUsuarios()
{
    return objetos;

}

vector<string> Ontologia::leerKeywords()
{
    return atributos;

}


