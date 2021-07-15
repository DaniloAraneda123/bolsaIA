import numpy as np
import pandas as pd
import tweepy as tw
import requests
import json
import tensorflow as tf
import tweepy as tw

class ApiTwitter:
    def __init__(self):
        self.consumer_key= 'ZKzf4t3PBtCmnj2sC1Wmy7ZxH'
        self.consumer_secret= 'g7Zr6Bwy92f4X5jC08L4d13MEcv1bo5Oo80ReiNCF7rcPX5hMa'
        self.access_token= '1399483654682910734-d0yiI4F0XUqZBGYKBKLsfVHD8JCtD2'
        self.access_token_secret= 'w7zCX4nDqF4oUV96MkAjYTH3e9eQr7lm3Gxd3sbYxuQqS'

        # self.consumer_key= 'gE3mJDFqpKJyKpkbTw0bQalhK'
        # self.consumer_secret= 'cMYp0aaOcV2qdT71Rxk8mPs7KAK1XatQzHtt9JwSrw3jJDoRGh'
        # self.access_token= '808780766406705153-ypEbG2h6PnsWkmAi8icd6axdvWOg42k'
        # self.access_token_secret= 'YRRauaXDbSInj8OoM6uy2PJjlxIg8TBVtxNKX4EcJrJmM'

        self.auth = tw.OAuthHandler(self.consumer_key, self.consumer_secret)
        self.auth.set_access_token(self.access_token, self.access_token_secret)
        self.api = tw.API(self.auth, wait_on_rate_limit=True)

    def getDFFromStatus(self,resultado):
        # Voy a crear diccionario

        dict = {}

        fecha = []
        retweet = []
        favoritas = []
        cuentas_favoritas = []
        cuentas_seguidores = []
        cuentas_amigos = []
        textos = []
        usuarios = []
        for tweet in resultado:
            fecha.append(tweet.created_at)
            retweet.append(tweet.retweet_count)
            favoritas.append(tweet.favorite_count)
            cuentas_favoritas.append(tweet.user.favourites_count)
            cuentas_seguidores.append(tweet.user.followers_count)
            cuentas_amigos.append(tweet.user.friends_count)
            if hasattr(tweet, 'retweeted_status'):                 
                textos.append(tweet.retweeted_status.full_text)             
            else:                 
                textos.append(tweet.full_text)              
            usuarios.append(tweet.user.id)
    
            dict["Fecha"] = fecha
            dict["Retweets"] = retweet
            dict["Favoritas"] = favoritas
            dict["cuentas_favoritas"] = cuentas_favoritas
            dict["cuentas_seguidores"] = cuentas_seguidores
            dict["cuentas_amigos"] = cuentas_amigos
            dict["texto"] = textos
            dict["usuario"] = usuarios
        return pd.DataFrame.from_dict(dict)

    def getTweets(self, json):
        # Parametros de la api
        lang = "en"
        # Parametros obtenido de la ontologia

        # Usuarios
        users = json["user"]
        users_query = ""
        for usuario in users:
            if(users_query==""):    
                users_query += "from:"+usuario
            else:
                users_query += " OR from:"+usuario
    
        # keywords
        keywords = json["keyword"]
        keywords_query = ""
        for keyword in keywords:
            if(keywords_query==""):    
                keywords_query += keyword
            else:
                keywords_query += " OR "+keyword

        # Recientes o Populares o Mixed
        result_type = json["result_type"]

        # Máximo número de tweets que esperamos
        max_tweets = json["max_tweets"]
        
        # QueryFinal
        query = users_query + " "+ keywords_query
        resultado = [status for status in tw.Cursor(self.api.search,q = query, 
            lang=lang, result_type=result_type,until=json["fecha"],
            tweet_mode="extended",retweeted_status="full_text").items(int(max_tweets))]
        df = self.getDFFromStatus(resultado)
        return df
