#include"../include/TrainingDocParser.h"

TrainingDocParser::TrainingDocParser()
{

}

TrainingDocParser::~TrainingDocParser()
{

}

void TrainingDocParser::SetDir(string & TrainingDir)
{
	m_dir=TrainingDir;

}


bool  TrainingDocParser::ParseDoc(string & fileName,string & docContent,string & finalCat,string & rootCat)
{
	string content;
	string dir=m_dir+"/"+fileName;

	ReadFile(dir,content);//read file

	/*extract field*/
	string CatNameBegin="QESTION_ClASSIFY:";
	string CatNameEnd="URL:";//qa data
	//string CatNameEnd="@CAPTION:";//news

	string CatTitleBegin="CAPTION:";
	string CatTitleEnd="DESCRIPTION:";

	string CatContentBegin="DESCRIPTION:";
	string CatContentEnd="RIGHT_ANSWER:";

	string CatAnswerBegin="RIGHT_ANSWER:";
	//string CatAnwserEnd="<answer>";

	string strName,strTitle,strContent,strAnswer;

	FindField(content,CatNameBegin,CatNameEnd,strName);
	FindField(content,CatTitleBegin,CatTitleEnd,strTitle);
	FindField(content,CatContentBegin,CatContentEnd,strContent);
	//FindField(content,CatAnswerBegin,CatAnwserEnd,strAnswer);
	int pos=content.find(CatAnswerBegin);
	if(pos!=-1)
	{
		strAnswer=content.substr(pos+CatAnswerBegin.length(),content.length()-pos-CatAnswerBegin.length());
	}

	docContent=strTitle+strContent+strAnswer;


	/*cat name parsing*/
	SplitCatDir(strName,finalCat,rootCat);

	return true;

}


bool TrainingDocParser::ParseDocDetail(string & fileName,string & question,string & finalCat)
{
	 string content;
	 string dir=m_dir+"/"+fileName;
	    
	 ReadFile(dir,content);//read file

	 /*extract field*/
	 string CatNameBegin="QESTION_ClASSIFY:";
	 string CatNameEnd="URL:";//qa data
	 //string CatNameEnd="@CAPTION:";//news
			  
	 string CatTitleBegin="CAPTION:";
	 string CatTitleEnd="DESCRIPTION:";
		   
	 string CatContentBegin="DESCRIPTION:";
	 string CatContentEnd="RIGHT_ANSWER:";
		     
	 string CatAnswerBegin="RIGHT_ANSWER:"; 

	 string strName,strTitle,strContent,strAnswer;
	   
	 FindField(content,CatNameBegin,CatNameEnd,strName);
	 FindField(content,CatTitleBegin,CatTitleEnd,strTitle);
	 FindField(content,CatContentBegin,CatContentEnd,strContent);
	 //FindField(content,CatAnswerBegin,CatAnwserEnd,strAnswer);

	 question=strTitle+strContent;//only question
	 
	 /*cat name parsing*/
	 string rootCat;
	 SplitCatDir(strName,finalCat,rootCat);

	 return true;


}


void TrainingDocParser::ReadFile(string & fileName,string & Content)
{
	ifstream fin(fileName.c_str());
	string line;
	 
	while(getline(fin,line))
	{
		Content=Content+line;
			
	}


}


bool TrainingDocParser::ParseJsonDoc(string & fileName,string & docContent,string & finalCat,string & rootCat)
{
	Json::Value root;
	Json::Reader reader;

	string dir=m_dir+"/"+fileName;

	//cout<<"before reading file"<<dir<<endl;

	std::ifstream file(dir.c_str());
	file>>root;
	//cout<<"after reading file"<<endl;

	/*
	//bool bSucc=reader.parse(dir,root);

	if(bSucc==false)
	{
		cout<<"Wrong in parse the json file:"<<dir<<endl;
		return false;

	}

	*/


	string strCat=root["QESTION_ClASSIFY"].asString();
	string strTitle=root["CAPTION"].asString();
	string strDesc=root["DESCRIPTION"].asString();
	string strAns=root["RIGHT_ANSWER"].asString();

	//cout<<"after parse field"<<endl;
	docContent=strTitle+strDesc+strAns;

	 /*cat name parsing*/                                                          
	SplitCatDir(strCat,finalCat,rootCat); 

//	cout<<docContent<<endl;
//	cout<<finalCat<<endl;
//	cout<<rootCat<<endl;

	return true;

}

void TrainingDocParser::FindField(string & Doc,string & beginTag,string & endTag,string & fieldContent)
{
	string targetStr=SS.ExtractPatternOnce(Doc,beginTag,endTag);
	fieldContent=targetStr;

}

bool  TrainingDocParser::SplitCatDir(string & catDir,string & leafCatName,string & rootCatName)
{
	string seperator=" ";
	vector<string> result;

	SS.Split(catDir,seperator,result);
	if(result.size()>0)
	{
		if(result.size()==1)//the flat cat
		{
			leafCatName=result.at(0);
			rootCatName="NULL";
			return true;
		}
		
		/*more than one level*/
		vector<string>::iterator it,it_guard;
		
		it_guard=result.end();
		it_guard--;
		leafCatName=(*it_guard);

		string tmp;
		for(it=result.begin();it!=result.end();it++)
		{
			if(it!=it_guard)//not the last level
			{
				tmp=tmp+(*it)+">";

			}
			

		}

		rootCatName=tmp;
		return true;
	}
	else//wrong,no any cat name
	{
		return false;
	}


}
