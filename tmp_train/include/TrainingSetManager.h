/**
 *purpose:to manage the training set
 *author:junlin zhang
 *date:2011-02-22
 *
 */

#ifndef TRAININGSETMAMAGER_H
#define TRAININGSETMAMAGER_H

#include"common_header.h"
#include"TrainingDocParser.h"
#include"SegmentSina.h"
#include"CatIdMap.h"
#include"FeatureConv.h"
#include"FeatureSelection.h"
#include"Config.h"
#include "superString.h"
//#include<map>

//using namespace std;


class TrainingSetManager{

	public:
		TrainingSetManager();
		~TrainingSetManager();

		bool Init(string & configFile);//load resource

		void TrainFeature();//to select the features from training set
		
		//void Convert2LibSvmFormat();//to convert the trainingset into livsvm input format

		void DragPullCentoriod();//to use drag and pull centoriod method to produce the model

	protected:

	   /*load resource*/
		bool LoadTrainingSet(string & dirListFile,vector<TrainingDocSeged> & trainingSetSeged);//loading the training set documents
		bool LoadFileList(string & dirListFile,vector<string> & fileList);//reading the file name list	
		void LoadAllDocs(vector<string> & fileNameList,vector<TrainingDoc> & trainingSet);
		
		bool LoadFileList2(string & dirListFile,multimap<string,string> & fileList);
		void LoadAllDocs2(multimap<string,string> & fileList,vector<TrainingDoc> & trainingSet);
		void LoadDoc(string & fileName,string & content);

		void SegmentDocs(vector<TrainingDoc> & trainingSet,vector<TrainingDocSeged> & trainingSetSeged);//segment training set
		void DeleteSameWord(vector<string> & wordList,vector<string> & wordListNoSame); 
		void CollectIDF(WPList & wordList,map<string,int> & idfInfo,vector<string> & seged);
		
		
		/*featureSelection*/
		void FeatureSelectionNow(vector<TrainingDocSeged> & trainingSetSeged,int topK,string & featureFile);
		void TurnFormat(vector<TrainingDocSeged> & trainingSetSeged,DocSet & docset);//format transfermation

	
		/*turn into libsvm samples*/
		void TurnIntoSamples(vector<TrainingDocSeged> & trainingSet,SampleSet & samples);
		bool  TurnOneSample(TrainingDocSeged & item,LibSvmSample & sample);
		void WriteSampleSet(SampleSet & samples,string & outputFile);//write trainging samples to files

		/*for centoriod */
		void Turn2CentoriodFormat(vector<TrainingDocSeged> & trainingSetSeged,DocsInFeature & newFormat,bool bSemantic);//turn format and norm the weight of each feature
		void SumCentoriodFeature(DocsInFeature & newFormat,CentoriodSum & sumValue);//sum the feature weight for each category's centoriod
		void TurnSimpleForm(CentoriodSum & sumValue,CatCentoriod & simpleCencoriod);//turn the format into simple one
		void NormSimpleForm(CatCentoriod & simpleCencoriod1,CatCentoriod & simpleCencoriod2);//norm the feature
		void Convert2Feathure(map<int,float> & featureSet,FeatureVec & fv);//turn format

		/*for pushdrag*/
		void PushDragOneDoc(CatCentoriod & sumWeight,CatCentoriod & normWeight,FeatureVec & document,int shouldBeCat,int indeedCat);//the push drag for one document,change the wumWeight and norm weight both for 2 category
		void AddingFeature(FeatureVec & centoriod,FeatureVec & document);//adding doc's feature to centoriod
		void ReducingFeature(FeatureVec & centoriod,FeatureVec & document);//adding doc's feature to centoriod
		void UpdateNormWeight(CatCentoriod & normWeight,int catIndex,FeatureVec & newFeatureVec);//update centoriod
		void ChangeFeature(FeatureVec & centoriod,FeatureVec & document,changeType & type);


		void OneIteration(DocsInFeature  & samples,CatCentoriod & sumWeight,CatCentoriod & normWeight);//one iteration for all document in training set
		int WhichCatNow(CatCentoriod & normWeight,FeatureVec & document);//which cat the document is for current centoriod
		float Similarity(FeatureVec & document,FeatureVec & centoriod);//similarity of tf.idf
		float Similarity2(FeatureVec & document,FeatureVec & centoriod);
		void WriteCentoriod(CatCentoriod & normWeigh,string & fileName);

	private:

		vector<string> m_fileList;//file list of training set
		vector<TrainingDoc> m_trainingSet;//the training set
		vector<TrainingDocSeged> m_trainingSetSeged;//the training set segmented

		map<string,int> m_idfInfo;//idf information

		SampleSet m_samples;//samples with libsvm format

		TrainingDocParser  m_docParser;//to parse the training set document
		segmentSina m_segmentor;//segmentor
		CatIdMap m_mapper;//cat name and id maping function
		FeatureConv m_featConvertor;//to convert a document into feature vector
		FeatureSelection  m_FS;//feature selection
		SuperString    m_SS;
	
		Config  m_config;//config 
		string dirListFile;
		string trainingDir;
		string stopwordFile;
		int topKFeature;
		string featureFile;
		string catIdMapFile;
		string libSvmInputFile;
		string lexiconpath;

	    /*drag and pull*/
		string m_centoriodFile;//the file name to store the centoriod of drag and pull method
		int    m_maxItTime;//max iterate time for drag and pull method
		float  m_dragPara;//drag parametor
		float  m_pushPara;//push parametor
		int    m_Semantic;

};
#endif
