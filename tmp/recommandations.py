#!/usr/bin/env python
# -*- coding:utf-8 -*-
# vim set expandtab
from math import sqrt

def loadMovieLens(path="./data/movielens"):
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


def sim_distance(dict1, dict2):
    si = [item for item in dict1 if item in dict2]

    if len(si) == 0: return 0
    sum_of_squares = sum(pow(dict1[item] - dict2[item], 2) for item in si)
    return 1 / (1+sqrt(sum_of_squares))


def topMatches(prefs, person, n=5, similarity=sim_distance):
    scores = []
    for other in prefs :
        if other!=person:
            scores.append((similarity(prefs[person], prefs[other]), other))

    scores.sort()
    scores.reverse()
    return scores[0:n]

def recommand(prefs, person, n=5, similarity = sim_pearson):
    totals = {}
    simSums = {}
    for other in prefs:
        if other is person:
            continue
        sim = similarity(prefs[person], prefs[other])

        if sim < 0: continue

        for item in prefs[other]:
            if item not in prefs[person] or prefs[person][item] == 0:
                totals.setdefault(item, 0)
                totals[item] += prefs[other][item] * sim

                simSums.setdefault(item, 0)
                simSums[item] += sim

    print len(totals)
    print len(simSums)

    rankings = [(total/simSums[item], item) for item, total in totals.items() if simSums[item] > 0]

    rankings.sort()
    rankings.reverse()

    return rankings[0:n]


def transformPrefs(prefs):
    result = {}
    for person in prefs:
        for item in prefs[person]:
            result.setdefault(item, {})
            result[item][person] = prefs[person][item] 
    return result

def sim_pearson(dict1, dict2):


    si = [item for item in dict1 if item in dict2]
    n = len(si)
    if not n:
        return 0

    sumXY = sum([dict1[item] * dict2[item] for item in si])
    sumX  = sum([dict1[item] for item in si])
    sumY  = sum([dict2[item] for item in si])
    sumX2 = sum([pow(dict1[item], 2) for item in si])
    sumY2 = sum([pow(dict2[item], 2) for item in si])


    numerator = n *sumXY - sumX * sumY
    denominator = sqrt(n * sumX2 - pow(sumX, 2)) * sqrt(n*sumY2 - pow(sumY, 2))
    if denominator == 0:
        return 0

    r = numerator / denominator
    return r

def relevancy(sort1, sort2):
    dict1 = {}
    for k,v in enumerate(sort1):
        dict1[v] = k
    dict2 = {}
    for k,v in enumerate(sort2):
        dict2[v] = k
    return sim_pearson(dict1, dict2) / 2 + 0.5


def sim_jaccob(dict1, dict2):
    si = [item for item in dict1 if item in dict2]
    factor = len(si)*1.0 / (len(prefs[u1])+ len(prefs[u2]) - len(si))
    return factor

