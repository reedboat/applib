/*
 *purpose:to map the cat id and cat name
 *author:junlin zhang
 *date:2011-02-22
 *
 */

#ifndef CATIDMAP_H
#define CATIDMAP_H

#include<string>
#include<vector>
#include<map>
#include<fstream>
#include<iostream>

using namespace std;

class CatIdMap{

	public:
		CatIdMap();
		~CatIdMap();

		/*query interface*/
		int  Name2ID(string & leafCatName);//map(leafCatName)-->cat id
		string ID2Name(int catID);//map(catID)--->leaf cat name
		string RootCatName(string & leafCatName);//map(leafCatName)-->rootCatName
		string ID2FullName(int catID);//map(catID)--->full name:root name+leaf cat name

		/*set the data*/
		int AddOneItem(string & rootCatName,string & leafCatName);//adding one item(etc,one traing doc,return the docID number

		/*write to the file*/
		void WriteCatInfo(string & outputFile);
		bool LoadCatInfo(string & inputFile);//reading the category information


	protected:

	private:

		int m_currentCatID;//the current category ID

		map<string,string> m_leafRootMap;//string1:leaf cat name string2:root cat name

		map<int,string> m_id2nameMap;//id 2 name
		map<string,int> m_name2idMap;//name 2 id

};
#endif
