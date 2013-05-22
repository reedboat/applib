#!/usr/bin/env python
# coding: utf-8

from segment import Segment


class Classfier():
    segmentor = None
    features={}
    categorys={}
    model={}

    def __init__(self, host, port):
        self.load()
        self.segmentor= Segment(host, port)

    def __del__(self):
        if self.segmentor:
            self.segmentor = None
        self.features = None 
        self.categorys = None
        self.model = None

    def load(self):
        feature_file='thrift/etc/feature.txt'
        category_file='thrift/etc/catid.txt'
        model_file='thrift/data/knn.model'

# load centoroid
        for line in open(model_file):
            line = line.strip()
            if not line:
                break
            index, rest=line.split('|')
            centoroid = {}
            for part in rest.split(" "):
                k, v = part.split(':')
                centoroid[int(k)] = float(v)
            self.model[int(index)] = centoroid
        
# load features
        for line in open(feature_file):
            line=line.strip()
            parts = line.split("\t")
            feature = {
                "index": int(parts[0]),
                "idf" : float(parts[2])
            }
            word = parts[1]
            self.features[word] = feature

# load category
        for line in open(category_file):
            line = line.strip()
            catid, leaf, root = line.split("\t")
            self.categorys[int(catid)] = leaf
    

    def predict(self, content):
        features = self.text2Feature(content)
        return self.maxProbCat(features);

    def segment(self, content):
        self.segmentor.connect()
        result  = self.segmentor.segment(content)
        self.segmentor.close()
        return self.segmentor.parse(result)

    def similarity(self, docFeatures, centoroid):
        value = 0.0
        for k,v in enumerate(docFeatures):
            item = docFeatures[v]
            if (centoroid.has_key(v)):
                value += item['weight'] * centoroid[v] 
        return value

    def text2Feature(self, content):
        keywords = self.segment(content)
        #print keywords
        features = {}
        for wordTf in keywords:
            word, tf = wordTf.split(':')
            tf = float(tf)
            if self.features.has_key(word):
                feature = self.features[word]
                index = feature["index"]
                idf   = feature["idf"]
                features[index] = {"word":word, "weight":tf*idf}
        #print features
        return features

    def maxProbCat(self, features):
        maxScore = 0.0
        probCat  = -1
        #docDotProduct = math.sqrt(sum(f.weight * f.weight for f in features))
        #if docDotProduct == 0:
            #return probCat

        for k, v in enumerate(self.model):
            score = self.similarity(features, self.model[v]);
            #print self.categorys[v], int(score * 10000)
            if score > maxScore:
                maxScore = score
                probCat = v

        label = self.categorys[probCat]
        return label


if __name__ == '__main__':
    classfier = Classfier("10.1.148.155", 12001)
    content  = '我相信南方供暖气将是中国的接下来的消费热点之一。我常对外国人说，去看看中国的南方的冬天吧，看看那些没有暖气的房间，你们就知道中国还是发展中国家了。冬天待在有暖气的房间里是中国南方民众的基本人权，无论有多少困难，实现它都应是中国现代化的最重要目标之一'
    print classfier.predict(content)
    content = '国务院决定从今年1月1日起按照10%的标准上调企业退休人员基本养老金，这已经是连续第9年上调企业养老金。此次上调之后，企业退休人员平均每月养老金将突破1800元。\
    《第一财经（微博)日报》从地方社保局了解到，目前上调养老金正式文件还没有正式下发到地方，但根据历年的惯例，东部省份将通过养老基金的收支来解决资金，中央将对中西部省份给予转移支付。\
    企业退休人员的养老金水平经过“九连涨”之后，已从2005年的月人均700元提升到今年的接近1900元。虽然未考虑通胀因素的名义上涨幅度不小，但由于筹资机制和替代率不同，企业退休人员和部分机关事业单位退休人员的养老金差距仍然明显。\
    此外，近一半省区养老金收不抵支，全国养老金个人账面亏空高达2万亿元的中长期问题，依然将对人口、财政、保险产业等政策组合提出挑战。而地区之间养老金收支不平衡现象将加大中西部一些地区对中央财政转移支付的依赖程度。'
    print '财经', classfier.predict(content)
    content = '记者近日了解到，目前，厦门各驾校C1(手动挡小车)的收费在6800元-7500元，C2(自动挡小车)的收费最高达8500元。 事实上，厦门的驾校仅有一两家的收费还在6000元左右徘徊，多数已经飙升到了7000元以上。此外，厦门绝大多数驾校都规定，40岁以上的女性和45岁以上的男性，驾培费要再加1000元。也就是说，这两类人如果学的是小车自动挡，每个人的学费是9500元，快突破万元大关了。而今，厦门的驾培费成为全国最贵。'
    print "社会 ", classfier.predict(content)
    content = '腾讯体育讯 带着领先10分的优势进入末节，却被对手以22-2攻势反扑，火箭遭遇遗憾败仗——北京时间1月10日，火箭客场79-88负于黄蜂。火箭结束5连胜，黄蜂则取得3连胜。哈登此战得到25分，连续14场25＋创造了火箭新的队史纪录。'
    print "体育 ", classfier.predict(content)
