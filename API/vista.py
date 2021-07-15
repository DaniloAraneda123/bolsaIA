from elasticsearch import Elasticsearch
import tensorflow_hub as hub 
import numpy as np
import matplotlib.pyplot as plt
from modulo_4 import calcularCentroides
from sklearn.decomposition import PCA
from sklearn.preprocessing import MinMaxScaler


def embed_text(tweets, embed):
    vectors = embed(tweets)
    return [vector.numpy().tolist() for vector in vectors]


def consultaEmbedding(query,client):
    embed = hub.load("https://tfhub.dev/google/universal-sentence-encoder-large/5")
    query_vector = embed_text([query],embed)[0]
    script_query = {
        "script_score": {
            "query": {"match_all": {}},
            "script": {
                "source": "cosineSimilarity(params.query_vector, doc['title_vector']) + 1.0",
                "params": {"query_vector": query_vector}
            }
        }
    }
    response = client.search(
        index="posts",
        body={
            "size": 10,
            "query": script_query,
            "_source": {"includes": ["title"]}
        }
    )
    list = []
    for hit in response["hits"]["hits"]:
        list.append({
            "score": hit["_score"],
            "titulo": hit["_source"]["title"]
        }
        )
    return list


def consultaTradicional(query,client):
    response = client.search(
        index="posts",
        body={
            "size":10,
            "query": {
                "multi_match": {
                    "query": query,
                    "fields": [
                        "title.analizado"
                    ]
                }
            },
            "_source": {
                "includes": ["title"]
            }
        }
    )

    list = []
    for hit in response["hits"]["hits"]:
        list.append({
            "score": hit["_score"],
            "titulo": hit["_source"]["title"]
        })
    return list


def consultarComparativa(texto):
    client = Elasticsearch(timeout=30)
    dict={}
    dict["tradicional"] = consultaTradicional(texto,client)
    dict["embeddings"]   = consultaEmbedding(texto,client)
    return dict

################################################################################################################
def graficarCentroides(texto):
    dict = calcularCentroides()
    embed = hub.load("https://tfhub.dev/google/universal-sentence-encoder-large/5")
    query_vector = embed_text([texto],embed)[0]
    list=[]
    list.append(dict["politica"])
    list.append(dict["ecologia"])
    list.append(dict["economia"])
    list.append(dict["exterior"])
    list.append(dict["social"])

    matriz=np.array(list)

    pca=PCA(n_components=2)
    pca.fit(matriz)

    list.append(query_vector)
    matriz=np.array(list)
    X_pca=pca.transform(matriz)

    labels = ['politica','ecologia','economia','exterior','social','query']

    plt.scatter(X_pca[:,0], X_pca[:,1])
    for i, label in enumerate(labels):
        plt.annotate(label, (X_pca[i][0], X_pca[i][1]))

    plt.savefig("vistas/figura.png")

    




