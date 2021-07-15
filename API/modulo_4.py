import json
from elasticsearch import Elasticsearch
from elasticsearch.helpers import bulk
import tensorflow_hub as hub
import numpy as np
import pandas as pd
from sklearn.decomposition import PCA
import pickle as pk

##### INDEXACION #####
# def index_data():
#     embed = hub.load("https://tfhub.dev/google/universal-sentence-encoder-large/5")
#     client = Elasticsearch(timeout=30)
#     client.indices.delete(index="centroides", ignore=[404], request_timeout = 30000)

#     with open("data/index.json") as index_file:
#         source = index_file.read().strip()
#         client.indices.create(index="centroides", body=source, request_timeout = 30000)

#     docs = []
#     with open("data/tweets.json") as data_file:
#         for line in data_file:
#             line = line.strip()
#             doc = json.loads(line)
#             docs.append(doc)

#         if docs:
#             index_batch(docs,client,embed)

#     client.indices.refresh(index="centroides")

# def index_batch(docs,client,embed):
#     tweets = [doc["tweet"] for doc in docs]
#     tweets_vectors = embed_text(tweets,embed)
#     requests = []
#     for i, doc in enumerate(docs):
#         request = doc
#         request["_op_type"] = "index"
#         request["_index"] = "centroides"
#         request["tweet_vector"] = tweets_vectors[i]
#         requests.append(request)
#     bulk(client, requests)

# ##### EMBEDDING #####
# def embed_text(tweets,embed):
#     vectors = embed(tweets)
#     return [vector.numpy().tolist() for vector in vectors]


##### CENTROIDES ####
def calcularCentroides():
    client = Elasticsearch(timeout=30)
    response = client.search(
        index="centroides",
        body={
            "size":40,
            "query": {"match_all":{}},
            "_source": {"includes": ["tweet_vector","etiqueta"]}
        }
    )

    etiquetas=[]
    vectores=[]
    for hit in response["hits"]["hits"]:
        etiquetas.append(hit["_source"]["etiqueta"])
        vectores.append(hit["_source"]["tweet_vector"])

    dict={}
    for etiqueta in ['politica','ecologia','economia','exterior','social']:
        matriz=[]
        for i in range(len(etiquetas)):
            if etiqueta==etiquetas[i]:
                matriz.append(vectores[i])
        dict[etiqueta]=np.mean(np.array(matriz),axis=0).tolist()    
    return dict

def calcularPCA(data):
    dict={}
    a=(data-np.array([1,2,3,4,5,6,7]))#agreggar medias
    pca = pk.load(open("RNN/pca.pkl",'rb'))
    X_pca=pca.transform(a)
    dict["pca"]=X_pca[0]
    return dict