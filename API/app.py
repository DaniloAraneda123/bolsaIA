from modulo_3 import Sentimiento
from modulo_4 import calcularCentroides,calcularPCA
from flask import Flask, request, jsonify
from flask_cors import CORS, cross_origin
from vista import consultarComparativa,graficarCentroides
import pandas as pd
import json
from API_twitter import ApiTwitter
from modulo_2 import Impacto
from embedding import filtrar_embedding
import gc

app = Flask(__name__)
CORS(app)

##########################################################################################
@app.route("/endpoint1", methods=['POST'])
def endpoint1():
    jsoncito = request.json
    print(jsoncito)

    tweet = ApiTwitter()
    dataFrame = tweet.getTweets(jsoncito)
    del tweet

    impac = Impacto()
    impac.getImpacto(dataFrame)
    del impac

    filtrar_embedding(dataFrame,1.1)
    dataFrame=dataFrame[['texto','embed','impacto','score']]

    senti = Sentimiento()
    senti.getSentimiento(dataFrame)
    del senti
    gc.collect()

    parsed = json.loads(dataFrame.to_json(orient="index"))
    parsed["numero"]=len(dataFrame)
    return json.dumps(parsed)

##########################################################################################
@app.route("/endpoint2", methods=['GET'])
def endpoint2():
    dict=calcularCentroides()
    return json.dumps(dict)

##########################################################################################
@app.route("/pca", methods=['POST'])
def endpoint3():
    jsoncito = request.json
    dict=calcularPCA(pd.DataFrame([jsoncito["datos"]]))
    return json.dumps(dict)

##########################################################################################
@app.route("/recuperarInformacion", methods=['POST'])
def recuperarInformacion():
    texto = request.json['texto']
    dict = consultarComparativa(texto)
    return jsonify(dict)

##########################################################################################
@app.route("/sentimiento", methods=['POST'])
def sentimiento():
    texto = request.json['texto']
    senti = Sentimiento()
    dict = senti.getSent(texto)
    return json.dumps(dict)

##########################################################################################
@app.route("/centroides", methods=['POST'])
def centroides():
    texto = request.json['texto']
    graficarCentroides(texto)
    return jsonify({"status":"ok"})

if __name__ == '__main__':
    app.run(host='localhost',debug=True, port=5000)