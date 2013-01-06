#include"../include/FeatureConv.h"

FeatureConv::FeatureConv()
{

}


FeatureConv::~FeatureConv()
{

}

bool FeatureConv::Init(string & featureFile,bool bSemantic)
{
	cout<<"featureFile Name="<<featureFile<<endl;
	if(bSemantic)
	{
		LoadFeatureFileSemantic(featureFile,m_FeatureSetSemantic);

	}
	else
	{

		LoadFeatureFile(featureFile,m_FeatureSet);
	}
	return true;

}

void FeatureConv::Convert(vector<string> & wordList,FeatureVec & featureFormat)
{
	map<string,int>  wordTf;
	map<int,CombinedWordItem> byIndex;

	CombineWord(wordList,wordTf);
	RankByIndex(wordTf,byIndex);
	ConvertFormat(byIndex,featureFormat);


}

void FeatureConv::ConvertNormSemantic(vector<string> & wordList,FeatureVec & featureFormat)
{
	   FeatureVec fv;

	  map<string,int>  wordTf;
	  map<int,CombinedWordItem> byIndex;
	     
	  CombineWord(wordList,wordTf);
	  RankByIndexSemantic(wordTf,byIndex);
	  ConvertFormat(byIndex,fv);

	   NormorizeFeature(fv,featureFormat);

}

void FeatureConv::ConvertNorm(vector<string> & wordList,FeatureVec & featureFormat)
{
	 FeatureVec fv;

	 map<string,int>  wordTf;
	 map<int,CombinedWordItem> byIndex;
	    
	 CombineWord(wordList,wordTf);
	 RankByIndex(wordTf,byIndex);
	 ConvertFormat(byIndex,fv);
    
     NormorizeFeature(fv,featureFormat);	 

}

void FeatureConv::NormorizeFeature(FeatureVec & featureFormat,FeatureVec & featureFormat2)
{
	float Z=0.0;

	FeatVecIte it;
	for(it=featureFormat.begin();it!=featureFormat.end();it++)
	{
		Z+=(*it).weight;//the weight is from tf*idf,but no normolization
	}

	if(Z==0.0)
	{
		Z=1.0;
	}

	for(it=featureFormat.begin();it!=featureFormat.end();it++)
	{
		int index=(*it).index;
		float weight=(float)((float)(*it).weight)/Z;	

		FeatureItem FI;
		FI.index=index;
		FI.weight=weight;

		featureFormat2.push_back(FI);
	}
}
bool FeatureConv::LoadFeatureFile(string & featureFile,map<string,FeatureSetItem> & FeatureSet)
{

	 ifstream fin(featureFile.c_str());                                              
	 string line;                                                                 
	                         
                                                                   
	
	 while(getline(fin,line))
	 {
		int index;
		string word;
		float idf;

		//fin>>index>>word>>idf;//There IS PROBLEM if the format is wrong of file
	
		if(ParseLine(line,index,word,idf)==true)
		{
		//	cout<<"index="<<index<<"word="<<word<<"idf="<<idf<<endl;

			FeatureSetItem fsi;
			fsi.index=index;
			fsi.idf=idf;

			pair<string,FeatureSetItem> value=make_pair(word,fsi);
			FeatureSet.insert(value);
		}
	 }

}

bool FeatureConv::LoadFeatureFileSemantic(string & featureFile,map<string,vector<FeatureSetItem> > & FeatureSet)
{
	 ifstream fin(featureFile.c_str());
	 string line;

	 while(getline(fin,line))
	 {
		int index;
		string word;                                                              
		float idf; 

		if(ParseLine(line,index,word,idf)==true)
		{

			 FeatureSetItem fsi;                                                   
			 fsi.index=index;                                                      
			 fsi.idf=idf;  

			 map<string,vector<FeatureSetItem> >::iterator it;
			 it=FeatureSet.find(word);
			 if(it!=FeatureSet.end())//find
			 {
				  
				  (*it).second.push_back(fsi);  

			 }
			 else
			 {
				 vector<FeatureSetItem> cache;
				 cache.push_back(fsi);

				 pair<string,vector<FeatureSetItem> > value=make_pair(word,cache);
				 FeatureSet.insert(value);

			 }
		}
	 }

}


bool FeatureConv::ParseLine(string & Line,int & index,string & word,float & idf)
{
	string seperator="\t";
	vector<string> splited;

	m_SS.Split(Line,seperator,splited);

	if(splited.size()!=3)
	{
		cout<<"Wrong in loading one feature item"<<endl;
		return false;
	}
	else
	{
		string strIndex=splited.at(0);
		string strWord=splited.at(1);
		string strIdf=splited.at(2);

		index=m_SS.Atoi(strIndex);
		word=strWord;
		idf=m_SS.Atof(strIdf);

		if(index<0)
			return false;

		return true;

	}


}


void FeatureConv::CombineWord(vector<string>  & wordList,map<string,int> & wordTf)
{
	wordTf.clear();

	vector<string>::iterator it;
	map<string,int>::iterator it2;
	for(it=wordList.begin();it!=wordList.end();it++)
	{
		    it2=wordTf.find((*it));
			if(it2!=wordTf.end())//found
			{
				(*it2).second+=1;//tf value add 1

			}
			else//new word
			{
				pair<string,int> value=make_pair((*it),1);
				wordTf.insert(value);


			}
	}

}

void FeatureConv::RankByIndex(map<string,int> & wordTf,map<int,CombinedWordItem> & byIndex)
{

	map<string,int>::iterator it;
	for(it=wordTf.begin();it!=wordTf.end();it++)
	{
		int index;
		float idf;
		string word=(*it).first;
		if(FindWordIndex(word,index,idf)==true)//found in feature set
		{
			CombinedWordItem cwi;
			cwi.word=(*it).first;
			cwi.tf=(*it).second;
			cwi.idf=idf;

			pair<int,CombinedWordItem> value=make_pair(index,cwi);
			byIndex.insert(value);
	
		}
		
	}
}


void FeatureConv::RankByIndexSemantic(map<string,int> & wordTf,map<int,CombinedWordItem> & byIndex)
{
	 map<string,int>::iterator it;
	 for(it=wordTf.begin();it!=wordTf.end();it++)
	 {                                                                             
		//int index;
		float idf;                                                                
		string word=(*it).first;
		

		vector<FeatureSetItem>  cache;
		vector<FeatureSetItem>::iterator it_f;

		if(FindWordIndexSemantic(word,cache)==true)//found in feature set
		{

			for(it_f=cache.begin();it_f!=cache.end();it_f++)
			{

				CombinedWordItem cwi;
				cwi.word=(*it).first;
				cwi.tf=(*it).second;
				cwi.idf=(*it_f).idf;//all same                                                          
				int index=(*it_f).index;//this index,the same word may belongs to many index

				/*do we have already the word in this semantic index?*/
				map<int,CombinedWordItem>::iterator it2;
				it2=byIndex.find(index);
				if(it2==byIndex.end())//no
				{			
					pair<int,CombinedWordItem> value=make_pair(index,cwi);
					byIndex.insert(value);
			   
					//cout<<"We have feature:"<<cwi.word<<"\tindex="<<index<<endl;	
				}
				else//we have,accumulating the weight
				{
					(*it2).second.tf+=cwi.tf;
					(*it2).second.idf+=cwi.idf;
					//cout<<"We update weight to"<<(*it2).second.idf<<"\t for word"<<cwi.word<<"\tweight="<<cwi.tf*cwi.idf<<endl;
				}	//end if		
				      
			}//end for
		}//end if
	 }	

}
void FeatureConv::ConvertFormat(map<int,CombinedWordItem> & byIndex,FeatureVec & featureFormat)
{
	/*the feature has been in order in byIndex*/
	map<int,CombinedWordItem>::iterator it;
	for(it=byIndex.begin();it!=byIndex.end();it++)
	{
		int tf=(*it).second.tf;
		float idf=(*it).second.idf;

		float weight=ComputeWeight(tf,idf);//compute the weight of feature i

		FeatureItem FI;
		FI.index=(*it).first;
		FI.weight=weight;

		featureFormat.push_back(FI);

	}

}

bool FeatureConv::FindWordIndex(string & word,int & index,float & idf)
{
	map<string,FeatureSetItem>::iterator it;
	it=m_FeatureSet.find(word);
	if(it!=m_FeatureSet.end())
	{
		index=(*it).second.index;
		idf=(*it).second.idf;

		return true;

	}
	else//not found
	{
		return false;
	}

}

bool FeatureConv::FindWordIndexSemantic(string & word,vector<FeatureSetItem>  & FeatureInfo)
{
	 map<string,vector<FeatureSetItem> >::iterator it;
	 it=m_FeatureSetSemantic.find(word);
	 if(it!=m_FeatureSetSemantic.end())
	 {
		FeatureInfo=(*it).second;
		return true;

	 }
	 else//not found
	 {
		return false;
	 }


}
/*whether we should add doclen here?*/
float FeatureConv::ComputeWeight(int & tf,float & idf)
{
	float score=tf*idf;
	//cout<<"weight="<<score<<endl;
	return score;


}

void FeatureConv::FilterFeatureWord(vector<string> & wordList,vector<string> & features)
{
		 
		map<string,int> wordTf;
		CombineWord(wordList,wordTf);//delete the same word
		 
		int index;
		float idf;
		 
		map<string,int>::iterator it;
		for(it=wordTf.begin();it!=wordTf.end();it++)
		{
			string word=(*it).first;
			if(FindWordIndex(word,index,idf)==true)//found in feature set
			{
				features.push_back(word);
						                                                                                  
			}                                                                         
						                                                                                   
		}                                                                             
}      
