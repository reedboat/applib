/*
 *purpose:to filter some feature which is not good
 *author:junlin zhang
 *date:2011-02-25
 *
 */

#ifndef FILTER_H
#define FILTER_H

#include<string>
#include<vector>
#include"superString.h"
#include<fstream>
#include<algorithm>

using namespace std;

class Filter{

	public:

		Filter();
		~Filter();

		void Init(string & filterWordFile);
		bool IsFilteredOfNot(string & word,int idf);

	protected:

		bool IsAllDigital(string & word);//whether all digital?
		bool IsTooShort(string & word);//whether too short?
		bool IsLowIdf(int idf);//whether too low idf?
		bool IsInFilterDict(string & word);//whether in filter list?

		void LoadDict(string & dictName,vector<string> & filterDict);

	private:

		vector<string> m_filterDict;//the words which is filtered
		SuperString m_SS;
};

#endif
