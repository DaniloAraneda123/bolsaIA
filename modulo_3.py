import numpy as np
import tensorflow as tf
import pandas as pd

class Sentimiento:
    
    def __init__(self):
        with open('RNN/model_config.json') as json_file:
            json_config = json_file.read()
        new_model = tf.keras.models.model_from_json(json_config)
        new_model.load_weights('RNN/path_to_my_weights')
        self.modelo=new_model
    
    def getSentimiento(self,dataFrame):
        resultados = self.modelo.predict(dataFrame['texto'].values)
        list=[]
        for i in range(len(resultados)):
            if resultados[i][0]<resultados[i][1]:
                list.append(1)
            else:
                list.append(0)
        dataFrame['sentimiento']=list

    def getSent(self,texto):
        resultados = self.modelo.predict(np.array([texto]))
        dict={}
        dict["texto"]=texto
        dict["positivo"]=float(resultados[0][1])
        dict["negativo"]=float(resultados[0][0])
        return dict
