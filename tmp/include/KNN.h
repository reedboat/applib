/**
 *purpose:to use knn to category a document
 *author:junlin zhang
 *date:2011-03-02
 */
#ifndef KNN_H
#define KNN_H

#include"FeatureConv.h"
#include"CatIdMap.h"
#include"Config.h"
#include"SegmentSina.h"
#include"superString.h"
#include"Timer.h"
#include "math.h"

#include<algorithm>
#include<numeric>

typedef struct _bestCatResult{
	int catIndex;//cat id
	string catName;//cat name
	float score;//final score;
}BestCatResult;

class KNN{

	public:
		KNN();
		~KNN();

		bool Init(string & configFile);//loading the resource


		int Predict(const string & content);//to predict the category
		float  Predict(const string & content,string & label);

		int PredictSemantic(string & content);//to predict the category with semantic features

		BestCatResult  PredictWithName(string & content);//to predict the category name
        void Text2FeatureWords(const string & content, vector<string> & feature);

	protected:
		bool LoadCenteroidVec(string & fileName);//loading the centeroid of each cat
		bool ParseLine(string & Line,vector<float> & centoroid,int & catIndex,int featureSize, float & featureDotProduct);

		void Text2Feature(string & content,FeatureVec & feature);//from document to feature
		void Text2FeatureSemantic(string & content,FeatureVec & feature);//from document to semantic features

		/**/
		int MaxProbCat(FeatureVec & document,vector<vector<float> > & Centoroid,float & score);//find the cat document belongs to
		string CatId2Name(int catId);//id-->name


		/*inner product of vec*/
		float InnerProduct(FeatureVec & document,vector<float> & centorid, int catIndex);//to compute the similarity of a document and a centorid

	private:
		int m_FeatureSize;//the feature size
		int m_CatNum;//the total cat number
		vector<vector<float> > m_centoroid;//the centorid vector list
		vector<float> m_centoroidFeatureDotProduct;//the centorid vector list
		int m_Semantic;//whether semantic feature	

		 CatIdMap m_catNameMap;//map:id<----->name
		 FeatureConv m_featConvertor;//convert the wordlist to feature and weight

		Config m_config;//config management                                       
		segmentSina m_segmentor;//segmentor
		SuperString m_SS;//string processing
		Timer       m_time;//timer		
		                                                                                     
		/*para in config file*/
		string m_modelName;//model name,which means the file contains the centoriod		
		string m_segPath;//segmentor path                                         
		string m_featureFile;//feature file name                                  
		string m_catIdMapFile;//cat id map      
};

#endif
