/*
 *purpose:to convert a document into the feature vector 
 *author:junlin zhang
 *date:2011-02-21
 */

#ifndef FEATURECONV_H
#define FEATURECONV_H

#include"common_header.h"

#include<string>
#include<vector>
#include<fstream>
#include<iostream>
#include"superString.h"


using namespace std;

class FeatureConv{

	public:
		FeatureConv();
		~FeatureConv();

		bool Init(string & featureFile,bool bSemantic);//loading the feature files

		void Convert(vector<string> & wordList,FeatureVec & featureFormat);//convert the doc into feature List,NOTE:wordList here:word plus idf


		void ConvertNorm(vector<string> & wordList,FeatureVec & featureFormat);//convert the doc into feature List AND Normorized,NOTE:wordList here:word plus idf

		void ConvertNormSemantic(vector<string> & wordList,FeatureVec & featureFormat);//convert the doc into semantic feature List AND Normorized,NOTE:wordList here:word plus idf
		
		void NormorizeFeature(FeatureVec & featureFormat,FeatureVec & featureFormat2);//Normorize feature

		void FilterFeatureWord(vector<string> & wordList,vector<string> & features);//filter the filters

	protected:

		
		bool LoadFeatureFile(string & featureFile,map<string,FeatureSetItem> & FeatureSet);//loading the feature file

		bool LoadFeatureFileSemantic(string & featureFile,map<string,vector<FeatureSetItem> > & FeatureSet);//loadingthe feature file

		bool ParseLine(string & Line,int & index,string & word,float & idf);//parse line


		void CombineWord(vector<string> & wordList,map<string,int> & wordTf);//combine the word in document
		void RankByIndex(map<string,int> & wordTf,map<int,CombinedWordItem> & byIndex);//rank the document by index number
		void RankByIndexSemantic(map<string,int> & wordTf,map<int,CombinedWordItem> & byIndex);//rank the document by index number
		
		void ConvertFormat(map<int,CombinedWordItem> & byIndex,FeatureVec & featureFormat);//convert into the target format

		bool FindWordIndex(string & word,int & index,float & idf);//find the m_FeatureSet for index and idf
		
		 bool FindWordIndexSemantic(string & word,vector<FeatureSetItem>  & FeatureInfo);//find the m_FeatureSet for index and idf

		float ComputeWeight(int & tf,float & idf);//compute the weight of feature

		//void NormorizeFeature(FeatureVec & featureFormat,FeatureVec & featureFormat2);//Normorize feature

	private:

		map<string,FeatureSetItem> m_FeatureSet;//the feature set,for maping from word to index and idf

		map<string,vector<FeatureSetItem> > m_FeatureSetSemantic;//the feature set,for semantic feature,one word can belong to different topic index
		SuperString m_SS;

};
#endif
