/**
 *purpose:the feature selection
 *author:junlin zhang
 *date:2011-02-22
 *
 */
#ifndef FEATURESELECTION_H
#define FEATURESELECTION_H

#include"common_header.h"
#include"ChiSquare.h"
#include<iostream>
#include<algorithm>
#include"Filter.h"

class FeatureSelection{

	public:
		FeatureSelection();
		~FeatureSelection();

		void Init(string & stopwordName);

		bool DoIt(DocSet & docset,int topFeature,string & outfileName,map<string,int> & idfInfo);//to select the feature

	protected:

		void OutputFeature(string & outfileName,int topK,multimap<float,string> &  candidateFeature,map<string,int> & idfInfo);//output the feature to a file

		void GetGlobalPara(DocSet & docset);//for totalDoc and catDocNum and wordDocNum
		void GetLocalPara(DocSet & docset,int catID, map<string,int> & wordDocNum4OneCat);//for local information

		void Select4OneCat(map<string,int> & wordDocNum4OneCat,int catIndex,map<string,float> & middleFeature);//select candidate feature for one category

		void RankFeature(map<string,float> & middleFeature,multimap<float,string> &  candidateFeature);//rank the feature according to the ChiSquare Score

	private:

		/*for chisquare computation*/
		int m_totalDoc;//how many doc totally
		map<int,int> m_catDocNum;//how many doc in each catgory,int1:cat index int2:doc number
		map<string,int> m_wordDocNum;//the total times a word accur in document set,string:word int:df
		map<string,int> m_wordDocNum4OneCat;//the times a word accur in one category,string:word,int:df in one category



		multimap<float,string> m_candidateFeature;//the candidate feature,float:chisquare score string:word
		map<string,float> m_middleFeature;//the middle data for candidateFeature

		ChiSquare m_csComputer;//to compute the x2 for one category
		
		Filter m_filter;//feature selection

};

#endif
