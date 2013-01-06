#include"../include/CatIdMap.h"

CatIdMap::CatIdMap()
{
	m_currentCatID=0;

}

CatIdMap::~CatIdMap()
{

}

int  CatIdMap::Name2ID(string & leafCatName)
{
	map<string,int>::iterator it;
	it= m_name2idMap.find(leafCatName);
	if(it!=m_name2idMap.end())
	{
		return (*it).second;
	}
	else
	{
		return -1;//special ID

	}

}

string CatIdMap::ID2Name(int catID)
{
	 map<int,string>::iterator it;
	 it=m_id2nameMap.find(catID);
	 if(it!=m_id2nameMap.end())
	 {
		return (*it).second;
	 }
	 else
	 {
		 string nullStr="";
		 return nullStr;

	 }

}

string CatIdMap::RootCatName(string & leafCatName)
{
	map<string,string>::iterator it;
	it=m_leafRootMap.find(leafCatName);
	if(it!=m_leafRootMap.end())
	{
		return (*it).second;

	}
	else
	{
		string nullStr="";
		return nullStr;

	}

}

string CatIdMap::ID2FullName(int catID)
{
	string nullStr="";

	string leafCatName=ID2Name(catID);
	if(leafCatName==nullStr)
		return nullStr;

	string rootCatName=RootCatName(leafCatName);
	if(rootCatName==nullStr)
		return nullStr;

	string fullName=rootCatName+">"+leafCatName;
	return fullName;

}

int CatIdMap::AddOneItem(string & rootCatName,string & leafCatName)
{
	string nullStr="";

	string rootName=RootCatName(leafCatName);
	/*we have it!*/
	if(rootName!=nullStr)
	{

		return Name2ID(leafCatName);
	}

	/*adding the item*/
	pair<string,string> value=make_pair(leafCatName,rootCatName);
	m_leafRootMap.insert(value);

	pair<string,int> value1=make_pair(leafCatName,m_currentCatID);
	m_name2idMap.insert(value1);

	pair<int,string> value2=make_pair(m_currentCatID,leafCatName);
	m_id2nameMap.insert(value2);

	m_currentCatID++;//to add the cat ID counter

	return (m_currentCatID-1);

}

void CatIdMap::WriteCatInfo(string & outputFile)
{
	string nullStr="";

	ofstream saveFile(outputFile.c_str()); 

	map<int,string>::iterator it;
	for(it=m_id2nameMap.begin();it!=m_id2nameMap.end();it++)
	{
		string rootCatName="NULL";//default

		saveFile<<(*it).first<<"\t"<<(*it).second<<"\t";
		rootCatName=RootCatName((*it).second);
		if(rootCatName==nullStr)
			rootCatName="NULL";

		saveFile<<rootCatName<<endl;
	}

	saveFile.close();

}

bool CatIdMap::LoadCatInfo(string & inputFile)
{
	/*clear out*/
	m_id2nameMap.clear();
	m_name2idMap.clear();
	m_leafRootMap.clear();

	/*reading*/
	fstream fin;                                                                
	fin.open(inputFile.c_str(),ios::in|ios::out); 

	while(!fin.eof())
	{
		int catID;
		string leafCatName;
		string rootCatName;
		fin>>catID>>leafCatName>>rootCatName;

		pair<string,string> value1=make_pair(leafCatName,rootCatName);
		m_leafRootMap.insert(value1);

		pair<string,int> value2=make_pair(leafCatName,catID);
		m_name2idMap.insert(value2);

		pair<int,string> value3=make_pair(catID,leafCatName);
		m_id2nameMap.insert(value3);

	}

}
