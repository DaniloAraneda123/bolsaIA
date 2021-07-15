import json
from elasticsearch import Elasticsearch
from elasticsearch.helpers import bulk
import pandas as pd
from glob import glob
import os
import tensorflow_hub as hub

# INDEXAR
# def crearTweetsBase():

#     embed = hub.load("https://tfhub.dev/google/universal-sentence-encoder-large/5")
#     client = Elasticsearch(timeout=30)
#     client.indices.delete(index="tweets", ignore=[404], request_timeout=30000)

#     with open("data/index_tweets.json") as index_file:
#         source = index_file.read().strip()
#         client.indices.create(index="tweets", body=source,request_timeout=30000)

#     archivos = glob("data/data/*")
#     docs = []
#     for file in archivos:
#         with open(file) as f:
#             for line in f:
#                 docs.append(json.loads(line))

#     indexar(docs, client, embed)
#     client.indices.refresh(index="tweets")


# def indexar(docs, client, embed):
#     tweets = [doc["text"] for doc in docs]
#     tweets_vectors = embed_text(tweets, embed)
#     requests = []
#     for i, doc in enumerate(docs):
#         request = {}
#         request["_op_type"] = "index"
#         request["_index"] = "tweets"
#         request["tweet"] = tweets[i]
#         request["tweet_vector"] = tweets_vectors[i]
#         requests.append(request)
#     bulk(client, requests)


##### EMBEDDING #####
def embed_text(tweets, embed):
    vectors = embed(tweets)
    return [vector.numpy().tolist() for vector in vectors]


#### MODULO FITRAR POR SIMILITUD ######
def filtrar_embedding(dataFrame,filtro):

    embed = hub.load("https://tfhub.dev/google/universal-sentence-encoder-large/5")
    client = Elasticsearch(timeout=30)

    lista = embed_text(dataFrame["texto"].tolist(), embed)

    scores=[]
    for embedding in lista:
        script_query = {
            "script_score": {
                "query": {"match_all": {}},
                "script": {
                    "source": "cosineSimilarity(params.query_vector, 'tweet_vector') + 1.0",
                    "params": {"query_vector": embedding}
                }
            }
        }

        response = client.search(
            index="tweets",
            body={
                "size": 5,
                "query": script_query,
                "_source": {"includes": ["tweet"]}
            }
        )
        scores.append(response["hits"]["hits"][0]["_score"])

    dataFrame['embed'] = [tuple(x) for x in lista]
    dataFrame['score']=scores
    dataFrame.query('score >= {}'.format(filtro),inplace=True)
    dataFrame.reset_index(inplace=True,drop=True)