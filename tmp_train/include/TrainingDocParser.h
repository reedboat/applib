/*
 *purpose:parse the training set document
 *author:junlin zhang
 *date:2011-02-22
 */

#ifndef TRAININGDOCPARSER_H
#define TRAININGDOCPARSER_H

#include"superString.h"
#include<string>
#include<fstream>

#include"json/json.h"

using namespace std;

class TrainingDocParser{

	public:
		TrainingDocParser();
		~TrainingDocParser();

		void SetDir(string & TrainingDir);

		bool ParseDoc(string & fileName,string & docContent,string & finalCat,string & rootCat);
		bool ParseDocDetail(string & fileName,string & question,string & finalCat);

		bool ParseJsonDoc(string & fileName,string & docContent,string & finalCat,string & rootCat);

	protected:

		void ReadFile(string & fileName,string & Content);
		void FindField(string & Doc,string & beginTag,string & endTag,string & fieldContent);//extract a field content
	

		bool SplitCatDir(string & catDir,string & leafCatName,string & rootCatName);

	private:
		string m_dir;//the dir for training set documents
		SuperString SS;
};

#endif
