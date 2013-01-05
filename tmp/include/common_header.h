/*
 *purpose:some common definement or micro
 *author:junlin zhang
 *date:2011-02-21
 */

#ifndef COMMON_HEADER_H
#define COMMON_HEADER_H

#include<string>
#include<vector>
#include<map>
#include<fstream>

using namespace std;


typedef struct _TrainingDocSeged{
	string leafCatName;//leaf cat name
	string rootCatName;//the root cat name such as:root/sport/basketball
	vector<string> wordList;//word list of doc

}TrainingDocSeged;//segmented training doc

typedef struct _TrainingDoc{
	string leafCatName;//leaf cat name
	string rootCatName;//the root cat name such as:root/sport/basketball
	string wordList;//word list of doc
		 
}TrainingDoc;//training doc not segmented

typedef struct _FeatureItem{

	int index;//feature index
	float weight;//feature weight

}FeatureItem;

typedef vector<FeatureItem> FeatureVec;//feature vector
typedef vector<FeatureItem>::iterator FeatVecIte;//iterator

typedef vector<vector<string> > DocInOneCat;//the document in a category
typedef vector<vector<string> >::iterator DocInCatIte;//the iterator

typedef map<int,DocInOneCat> DocSet;//the document set,int:catID,DocInOneCat:the document in a category
typedef map<int,DocInOneCat>::iterator DocSetIte;//iterator

/*training set:(x,y)*/
typedef struct _LibSvmSamlpe{
	int catLable;    //y value
	FeatureVec features;//x value
}LibSvmSample;

typedef  vector<LibSvmSample> SampleSet;
typedef  vector<LibSvmSample>::iterator SampleSetIte;


typedef struct _FeatureSetItem{

	int index;//feature index number
	float idf;//feature idf

}FeatureSetItem;



typedef struct _CombinedWordItem{

	string word;
	int tf;
	float idf;

}CombinedWordItem;

#endif
