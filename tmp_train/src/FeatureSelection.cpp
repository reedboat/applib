#include"../include/FeatureSelection.h"

FeatureSelection::FeatureSelection()
{

}

FeatureSelection::~FeatureSelection()
{

}

void FeatureSelection::Init(string & stopwordName)
{
	m_filter.Init(stopwordName);
	

}


bool FeatureSelection::DoIt(DocSet & docset,int topFeature,string & outfileName,map<string,int> & idfInfo)
{
	/*get the parameter*/
	GetGlobalPara(docset);

	cout<<"after get global parameter"<<endl;

	/*iterator each category*/
	DocSetIte it;
	for(it=docset.begin();it!=docset.end();it++)
	{
		int catID=(*it).first;

		m_wordDocNum4OneCat.clear();//clear the previous content
		GetLocalPara(docset,catID,m_wordDocNum4OneCat);//get the statistics for current cat id
		Select4OneCat(m_wordDocNum4OneCat,catID,m_middleFeature);//select features for current cat id
	
		cout<<"finish one cat's feature computing"<<endl;	
	}

	/*rank the score*/
	cout<<"begin ranking scores of feature"<<endl;
	RankFeature(m_middleFeature,m_candidateFeature);
	cout<<"after ranking"<<endl;

	/*write to the file*/
	OutputFeature(outfileName,topFeature,m_candidateFeature,idfInfo);
	cout<<"have wrotten the feature file"<<endl;


}

void FeatureSelection::RankFeature(map<string,float> & middleFeature,multimap<float,string> &  candidateFeature)
{
	map<string,float>::iterator it;
	for(it=middleFeature.begin();it!=middleFeature.end();it++)
	{
		pair<float,string> value=make_pair((*it).second,(*it).first);
		candidateFeature.insert(value);

	}

}

void FeatureSelection::OutputFeature(string & outfileName,int topK,multimap<float,string> & candidateFeature,map<string,int> & idfInfo)
{
	ofstream saveFile(outfileName.c_str());
	int counter=0;

	map<string,int>::iterator it;
	multimap<float,string>::reverse_iterator rev_it;
	for(rev_it=candidateFeature.rbegin();counter<topK && rev_it!=candidateFeature.rend();rev_it++)
	{
		int idf=-1;
		it=idfInfo.find((*rev_it).second);
		if(it!=idfInfo.end())
		{
			idf=(*it).second;

		}
		
		if(m_filter.IsFilteredOfNot((*rev_it).second,idf)==false)//not filtered
		{
			saveFile<<counter<<"\t"<<(*rev_it).second<<"\t"<<idf<<endl;
			counter++;
		}

	}

	saveFile.close();
}


void FeatureSelection::GetGlobalPara(DocSet & docset)
{
	/*iterator each category*/
	DocSetIte it;
	m_totalDoc=0;
	for(it=docset.begin();it!=docset.end();it++)
	{
		m_totalDoc+=(*it).second.size();//counting the total doc number
		pair<int,int> value=make_pair((*it).first,(*it).second.size());
		m_catDocNum.insert(value);//adding the document number of one category

		/*for each document in one category:iterator*/
		DocInCatIte dic_it;
		map<string,int>::iterator it2;
		for(dic_it=(*it).second.begin();dic_it!=(*it).second.end();dic_it++)
		{
			/*for each word in one document:NOTE HERE:NO SAME WORD IN DOC!*/
			vector<string>::iterator it_doc;
			for(it_doc=(*dic_it).begin();it_doc!=(*dic_it).end();it_doc++)
			{
				string word=(*it_doc);
				it2=m_wordDocNum.find(word);
				if(it2!=m_wordDocNum.end())//we have this word
				{
					(*it2).second+=1;

				}
				else//new word
				{
					pair<string,int> value1=make_pair(word,1);
					m_wordDocNum.insert(value1);

				}
							

			}
			


		}

	}

}

/*NOTE:this computation can be included in GetGlobalPara() to make the program faster*/
void FeatureSelection::GetLocalPara(DocSet & docset, int catID,map<string,int> & wordDocNum4OneCat)
{
	wordDocNum4OneCat.clear();

	DocSetIte dsi_it=docset.find(catID);
	if(dsi_it!=docset.end())
	{
		/*iterator each document in this category*/
		DocInCatIte dic_it;
		for(dic_it=(*dsi_it).second.begin();dic_it!=(*dsi_it).second.end();dic_it++)
		{
			 /*for each word in one document:NOTE HERE:NO SAME WORD IN DOC!*/
			 vector<string>::iterator it_doc;
			 map<string,int>::iterator it2;
			 for(it_doc=(*dic_it).begin();it_doc!=(*dic_it).end();it_doc++)
			 {
				string word=(*it_doc);
				it2=wordDocNum4OneCat.find(word);
				if(it2!=wordDocNum4OneCat.end())
				{
					(*it2).second+=1;
				}
				else
				{
					pair<string,int> value=make_pair(word,1);
					wordDocNum4OneCat.insert(value);

				}
				

			 }

			


		}


	}
	else//wrong
	{
		cout<<"something wrong:why we can't find category index:"<<catID<<"?"<<endl;
	}
	

}


void FeatureSelection::Select4OneCat(map<string,int> & wordDocNum4OneCat,int catIndex,map<string,float> &  middleFeature)
{
	float A=0.0;//the document number which contain a word in this cat
	float B=0.0;//the document number which NOT contain a word in this cat
	float D=0.0;//the document number which contain a word BESIDES this cat
	float E=0.0;//the document number which NOT contain a word BESIDES this cat
	
	float P=0.0;//the probability of this category:p(ci)=C(ci)/totalDocNum;

	multimap<float,string> topK;//top keyword in this cat

	/*find out doc number of the category*/
	float catDocNumber=0.0;
	float totalDocNum=(float)m_totalDoc;//total document number in training set

	map<int,int>::iterator it2;
	it2=m_catDocNum.find(catIndex);
	if(it2!=m_catDocNum.end())
	{
		catDocNumber=(*it2).second;//document number in the category

	}
	else
	{
		cout<<"something wrong:why we can't find category index:"<<catIndex<<"?"<<endl;
		return;
	}
	

	/*probability of this cat!*/
	P=(float)((float)catDocNumber/(float)totalDocNum);
	
		
		
		
	/*for each word in this category*/
	map<string,int>::iterator it;
	for(it=wordDocNum4OneCat.begin();it!=wordDocNum4OneCat.end();it++)
	{
		/*prepare the parameters for chisqare*/
		A=(*it).second;//the document number which contain a word in this cat
		D=catDocNumber-A;//the document number which contain a word BESIDES this cat
		
		int totalDoc4Word=0;
		map<string,int>::iterator it_dict;
		it_dict=m_wordDocNum.find((*it).first);
		if(it_dict!=m_wordDocNum.end())//should be this
		{
			totalDoc4Word=(*it_dict).second;

		}
		else
		{
			totalDoc4Word=0;//special value

		}
		
		B=totalDoc4Word-A;//the document number which  contain a word BESIDES  this cat
		
		E=totalDocNum-catDocNumber-B;//the document number which NOT contain a word BESIDES this cat

		
		/*chisquare*/
		float subScore=m_csComputer.Compute(A,B,D,E,totalDocNum,catDocNumber);//chisquare 
		//float ProbScore=P*subScore;//p(ci)*chiSquare
        float ProbScore=subScore;

		//cout<<"word="<<(*it).first<<"\t"<<"A="<<A<<"\t"<<"B="<<B<<"D="<<D<<"E="<<E<<"TotalDocNum="<<totalDocNum<<"catDocNumber="<<catDocNumber<<" and X2="<<subScore<<endl;


		pair<float,string> value=make_pair(subScore,(*it).first);
		topK.insert(value);

		/*update the chisquare score with other category*/
		map<string,float>::iterator it_map;
		it_map=middleFeature.find((*it).first);
		if(it_map!=middleFeature.end())//found
		{
			/*method 1:linear avg of each category*/
			(*it_map).second+=ProbScore;
			

			/*method2:get max in one of the category*/
		/*	if(ProbScore>(*it_map).second)
			{
				(*it_map).second=ProbScore;
			}
		*/	


		}
		else//newcomer
		{
			pair<string,float> value=make_pair((*it).first,ProbScore);
			middleFeature.insert(value);

		}

	}//end for


	/*output the top keyword in this cat:for debug only*/
	multimap<float,string>::reverse_iterator rit;
	int counter=0;
	
	cout<<"catIndex="<<catIndex<<endl;

	for(rit=topK.rbegin();rit!=topK.rend()&&counter<2000;rit++)
	{
		map<string,int>::iterator it;

		int idf=70;

		if(m_filter.IsFilteredOfNot((*rit).second,idf)==false)//not filtered )
		{
			cout<<(*rit).second<<"\t"<<(*rit).first<<endl;
			counter++;
		}
	}
   

}
