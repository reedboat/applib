/**
 *purpose:to segment the traing set into:traing set/tuning set/test set
 *author:junlin zhang
 *date:2011-03-08
 *
 */

#ifndef SEGTRAININGSET_H
#define SEGTRAININGSET_H

#include<string>
#include<vector>
#include<iostream>

#include"Config.h"


using namespace std;

class SegTraining{

	public:
		SegTraining();
		~SegTraining();

		bool Init(string & configFile);
		

		void ThreePart();//segment the training set into 3 part
		void TwoPart();//segment the training set into 3 part

	protected:
		
		void SetRoot(string & root);

		bool Seg2Part();//segment into two part
	    bool Seg3Part();//segment into three part	

		bool WriteFile(vector<string> & fileList,string & outFile);//write file list into output file
		bool LoadFile(string & inputFile,vector<string> & fileList);//load the filelist

	private:
		string m_root;//root path
		string m_allFileName;//all files list

		string m_trainingFileName;//training set file name,under the m_root path
		string m_tuningFileName;//tuning file name,under the m_root path
		string m_testFileName;//test file name,under the m_root path

		vector<string> m_allFiles;//all files name
		vector<string> m_trainingFiles;//training files name
		vector<string> m_tuningFiles;//tuing files name
		vector<string> m_testFiles;//test files name


		Config  m_config;//config

};

#endif
