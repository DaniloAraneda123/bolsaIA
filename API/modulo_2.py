import numpy as np
import tensorflow as tf
import pandas as pd
from sklearn import preprocessing
from pickle import load


class Impacto:

    def __init__(self):

        #self.ruta = '/'
        with open('NNFF/modelo.json') as json_file:
            json_config = json_file.read()
        self.new_model = tf.keras.models.model_from_json(json_config)
        self.new_model.load_weights('NNFF/modelo')
        self.min_max_scaler = load(open('NNFF/scaler.pkl', 'rb'))
        self.min_max_scaler.clip = False


    def getImpacto(self,dataFrame):
        df_aux = dataFrame.copy()
        df_aux.drop(columns=['Fecha', 'texto'], inplace=True)
        x = df_aux.values #returns a numpy array
        x_scaled = self.min_max_scaler.transform(x)
        dataFrameNormalizado = pd.DataFrame(x_scaled)
        resultados = self.new_model.predict(dataFrameNormalizado.values)
        dataFrame['impacto']=resultados