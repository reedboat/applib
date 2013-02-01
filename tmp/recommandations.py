#!/usr/bin/env python
# -*- coding:utf-8 -*-
# vim set expandtab
from math import sqrt

def loadMovieLens():
    path="./movielens"
    movies = {}
    for line in open(path + "/u.item"):
        id, title = line.split("|")[0:2]
        movies[id] = title

    prefs = {}
    for line in open(path + "/u.data"):
        user, movie, rating, ts = line.split('\t')
        prefs.setdefault(user, {})
        prefs[user][movies[movie]] = float(rating)
    return prefs


def simDistance(prefs, u1, u2):
    si = {}
    for item in prefs[u1]:
        if item in prefs[u2]:
            si[item] = 1

    if len(si) == 0: return 0
    sum_of_squares = sum(pow(prefs[u1][item] - prefs[u2][item], 2) for item in si)
    return 1 / (1+sqrt(sum_of_squares))


def topMatches(prefs, person, n=5, similarity=simDistance):
    scores = []
    for other in prefs :
        if other!=person:
            scores.append((similarity(prefs, person, other), other))

    scores.sort()
    scores.reverse()
    return scores[0:n]


def recommand(prefs, person, n):
    #users = topMatches(prefs, person, n=10)
    #for score, other in users:
        #score *pres[other][item] for item in prefs[other]
    pass

def relevancy(sort1, sort2):
    dict1 = {}
    for k,v in enumerate(sort1):
        dict1[v] = k
    dict2 = {}
    for i in enumerate(sort2):
        dict2[v] = k
    total = sum(pow(dict1[s] - dict2[s], 2) for s in dict1 if s in dict2)
    return sqrt(total) /sqrt(sum(pow(s,2) for s in dict1) * sum(pow(s,2) for s in dict2))




