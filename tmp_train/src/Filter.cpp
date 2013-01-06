#include"../include/Filter.h"

Filter::Filter()
{

}

Filter::~Filter()
{

}

void Filter::Init(string & filterWordFile)
{
	LoadDict(filterWordFile,m_filterDict);

	//cout<<"stopword size="<<m_filterDict.size()<<endl;
}

bool Filter::IsFilteredOfNot(string & word,int idf)
{
	if(IsTooShort(word)==true ||IsAllDigital(word)==true||IsInFilterDict(word)==true||IsLowIdf(idf)==true)
	{
		//cout<<"word="<<word<<"is filtered"<<endl;

		return true;

	}
	else
	{
		return false;
	}

}


bool Filter::IsTooShort(string & word)
{
	string newOne=m_SS.LTrip(word);

	if(m_SS.IsAllEN(newOne)==true)//we don't care about english
	{
		if(newOne.length()<2)
		{
			return true;

		}
		else
		{
			return false;

		}
		
	}
	else
	{
		if(newOne.length()<3)//single HZ
		{
			return true;

		}
		else
		{
			return false;

		}

	}

}


bool Filter::IsAllDigital(string & word)
{
	return m_SS.IsDigital(word);
}


bool Filter::IsLowIdf(int idf)
{
	if(idf<45)//is seems that the value 70 is the word segmentor can't recgonized
	{
		
		return true;
	}
	else
	{
		return false;

	}

}

bool Filter::IsInFilterDict(string & word)
{
	vector<string>::iterator it;
	it=find(m_filterDict.begin(),m_filterDict.end(),word);
	if(it!=m_filterDict.end())
	{
		//cout<<"is in filter dict"<<endl;
		return true;
	}
	else
	{
		return false;


	}

}

void Filter::LoadDict(string & dictName,vector<string> & filterDict)
{
	 ifstream fin(dictName.c_str());                                               
	 string line;                                                                  
	                                                                                      
	 while(getline(fin,line))                                                      
	 {                                                                             
		filterDict.push_back(line);                                                     
				                                                                                      
	 }                     


}


