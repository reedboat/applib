#include"../include/TrainingSetManager.h"

TrainingSetManager::TrainingSetManager()
{

}

TrainingSetManager::~TrainingSetManager()
{

}


bool TrainingSetManager::Init(string & configFile)
{
    /*read config file*/
    m_config.ReadFile(configFile);
    dirListFile=m_config.Read("fileListFile",dirListFile);
    trainingDir=m_config.Read("trainingSetDir",trainingDir);
    stopwordFile=m_config.Read("stopWordFile",stopwordFile);
    topKFeature=m_config.Read("featureNum",5000);
    featureFile=m_config.Read("outputFeature",featureFile);
    catIdMapFile=m_config.Read("catIdNameMap",catIdMapFile);
    libSvmInputFile=m_config.Read("libsvmFormatFile",libSvmInputFile);
    lexiconpath=m_config.Read("segmentPath",lexiconpath);

    m_Semantic=m_config.Read("semanticOrNot",0);

    m_dragPara=m_config.Read("dragParametor",1.0);
    m_pushPara=m_config.Read("pushParametor",1.0);
    m_maxItTime=m_config.Read("maxIteratorTimes",5);
    m_centoriodFile=m_config.Read("centoriodFile",m_centoriodFile);

    /*set training set root dir*/
    m_docParser.SetDir(trainingDir);

    /*read the stopword files for feature filter*/
    m_FS.Init(stopwordFile);

    /*segment inital*/

    string type="segpos";//"segment and idf";                                            
    bool bi= m_segmentor.Initial(lexiconpath,type);                             
    if(!bi)                                                                       
    {                                                                                            
        cout<<"segmentor initial flase"<<endl;                                    

    }      

    cout<<"after initialize the system!"<<endl;

    return bi;
}

void TrainingSetManager::TrainFeature()
{
    /*loading the training set*/                                                  
    m_trainingSetSeged.clear();                                                   
    LoadTrainingSet(dirListFile,m_trainingSetSeged);

    /*feature selection*/
    FeatureSelectionNow(m_trainingSetSeged,topKFeature,featureFile);

    /*write cat name id map*/
    m_mapper.WriteCatInfo(catIdMapFile);


}



void TrainingSetManager::TurnIntoSamples(vector<TrainingDocSeged> & trainingSet,SampleSet & samples)
{
    vector<TrainingDocSeged>::iterator it;
    for(it=trainingSet.begin();it!=trainingSet.end();it++)
    {
        LibSvmSample sample;
        if(TurnOneSample((*it),sample)==true)
        {
            samples.push_back(sample);
        }
    }

}


bool  TrainingSetManager::TurnOneSample(TrainingDocSeged & item,LibSvmSample & sample)
{
    string leafCatName=item.leafCatName;
    int catID=m_mapper.Name2ID(leafCatName);
    //	cout<<"CatID="<<catID<<endl;

    if(catID!=-1)
    {

        FeatureVec featureV;
        m_featConvertor.Convert(item.wordList,featureV);
        sample.catLable=catID;
        sample.features=featureV;

        return true;

    }
    else
    {
        cout<<"something wrong:we can't find the catID for cat Name:"<<leafCatName<<endl;
        return false;
    }


}


void TrainingSetManager::WriteSampleSet(SampleSet & samples,string & outputFile)
{
    ofstream saveFile(outputFile.c_str());

    SampleSetIte ssi_it;
    for(ssi_it=samples.begin();ssi_it!=samples.end();ssi_it++)
    {
        /*y*/
        saveFile<<(*ssi_it).catLable<<" ";

        /*x*/
        FeatVecIte fv_it;
        for(fv_it=(*ssi_it).features.begin();fv_it!=(*ssi_it).features.end();fv_it++)
        {
            saveFile<<(*fv_it).index<<":"<<(*fv_it).weight<<" ";
        }

        saveFile<<endl;//one line
    }

    saveFile.close();
}

bool TrainingSetManager::LoadTrainingSet(string & dirListFile,vector<TrainingDocSeged> & trainingSetSeged)
{
    /*clear the thing*/
    m_fileList.clear();
    m_trainingSet.clear();


    /*loading the resource*/
    /*	LoadFileList(dirListFile,m_fileList);
        LoadAllDocs(m_fileList,m_trainingSet);
     */
    multimap<string,string> fileList;
    LoadFileList2(dirListFile,fileList);
    LoadAllDocs2(fileList,m_trainingSet);

    cout<<"after loading the files and file size="<<m_trainingSet.size()<<endl;

    vector<TrainingDocSeged> cache;
    SegmentDocs(m_trainingSet,cache);

    m_trainingSet.clear();//now we don't need original documents any more,so release it!
    cout<<"after segment the training set"<<endl;

    /*delete the same word in a document*/
    vector<TrainingDocSeged>::iterator it;
    for(it=cache.begin();it!=cache.end();it++)
    {

        /*delete the same*/
        vector<string> noSame;
        DeleteSameWord((*it).wordList,noSame);

        TrainingDocSeged item;
        item.leafCatName=(*it).leafCatName;
        item.rootCatName=(*it).rootCatName;
        item.wordList=noSame;

        trainingSetSeged.push_back(item);

    }

    cout<<"after delete the same word in document"<<endl;

    return true;

}

bool TrainingSetManager::LoadFileList(string & dirListFile,vector<string> & fileList)
{
    //	cout<<"fileName="<<dirListFile<<endl;

    ifstream fin(dirListFile.c_str());                                             
    string line;                                                                

    while(getline(fin,line))
    {
        fileList.push_back(line);

    }	

}

bool TrainingSetManager::LoadFileList2(string & dirListFile,multimap<string,string> & fileList)
{
    ifstream fin(dirListFile.c_str());                                             
    string line;

    string seperator="\t";
    while(getline(fin,line))
    {
        vector<string> part;
        m_SS.Split(line,seperator,part);
        if(part.size()==2)
        {
            string catName=part.at(0);
            string fileName=part.at(1);

            pair<string,string> value=make_pair(catName,fileName);
            fileList.insert(value);

        }


    } 

}


void TrainingSetManager::LoadAllDocs2(multimap<string,string> & fileList,vector<TrainingDoc> & trainingSet)
{
    multimap<string,string>::iterator it;
    for(it=fileList.begin();it!=fileList.end();it++)
    {
        string content;
        LoadDoc((*it).second,content);

        TrainingDoc td;                                                           
        td.leafCatName=(*it).first;                                               
        td.rootCatName="NULL";                                               
        td.wordList=content; 

        trainingSet.push_back(td); 

    }

}

void TrainingSetManager::LoadDoc(string & fileName,string & content)
{
    ifstream fin(fileName.c_str());
    string line;

    while(getline(fin,line))
    {
        content=content+line;


    }


}


void TrainingSetManager::LoadAllDocs(vector<string> & fileNameList,vector<TrainingDoc> & trainingSet)
{
    vector<string>::iterator it;
    for(it=fileNameList.begin();it!=fileNameList.end();it++)
    {
        string content;
        string leafCatName;
        string rootCatName;

        //cout<<"before json parser"<<endl;

        //m_docParser.ParseJsonDoc((*it),content,leafCatName,rootCatName);
        m_docParser.ParseDoc((*it),content,leafCatName,rootCatName);

        //	cout<<"file Name="<<(*it)<<"content="<<content<<"leafName="<<leafCatName<<"rootName="<<rootCatName<<endl;

        //cout<<"file Name="<<(*it)<<endl;

        TrainingDoc td;
        td.leafCatName=leafCatName;
        td.rootCatName=rootCatName;
        td.wordList=content;

        trainingSet.push_back(td);
    }

}

void TrainingSetManager::SegmentDocs(vector<TrainingDoc> & trainingSet,vector<TrainingDocSeged> & trainingSetSeged)
{
    vector<TrainingDoc>::iterator it;
    for(it=trainingSet.begin();it!=trainingSet.end();it++)
    {
        vector<string> seged;
        WPList segIdf;
        m_segmentor.SegmentIdf((*it).wordList,segIdf);//segment and idf info
        CollectIDF(segIdf,m_idfInfo,seged);//collecting idf information

        TrainingDocSeged td;
        td.leafCatName=(*it).leafCatName;
        td.rootCatName=(*it).rootCatName;
        td.wordList=seged;

        trainingSetSeged.push_back(td);

    }

}

void TrainingSetManager::FeatureSelectionNow(vector<TrainingDocSeged> & trainingSetSeged,int topK,string & featureFile)
{
    DocSet docset;
    TurnFormat(trainingSetSeged,docset);

    cout<<"after tunformat"<<endl;

    m_FS.DoIt(docset,topK,featureFile,m_idfInfo);

}

void TrainingSetManager::TurnFormat(vector<TrainingDocSeged> & trainingSetSeged,DocSet & docset)
{
    vector<TrainingDocSeged>::iterator it;
    for(it=trainingSetSeged.begin();it!=trainingSetSeged.end();it++)
    {
        string leafCatName=(*it).leafCatName;
        string rootCatName=(*it).rootCatName;

        int catID=m_mapper.AddOneItem(rootCatName,leafCatName);
        if(catID!=-1)
        {
            DocSetIte dsi_it;
            dsi_it=docset.find(catID);
            if(dsi_it!=docset.end())//found,adding into
            {
                (*dsi_it).second.push_back((*it).wordList);

            }
            else//new catID
            {
                DocInOneCat cache;
                cache.push_back((*it).wordList);

                pair<int,DocInOneCat> value=make_pair(catID,cache);
                docset.insert(value);

            }

        }
        else
        {
            cout<<"something wrong:we can't find catID in TrainingSetManager::TurnFormat() function"<<endl;
        }

    }

}

void TrainingSetManager::DeleteSameWord(vector<string> & wordList,vector<string> & wordListNoSame)
{
    map<string,int> cache;//temp cache
    map<string,int>::iterator it2;

    vector<string>::iterator it;
    for(it=wordList.begin();it!=wordList.end();it++)
    {
        it2=cache.find((*it));
        if(it2==cache.end())
        {
            wordListNoSame.push_back((*it));

        }



    }

}

void TrainingSetManager::CollectIDF(WPList & wordList,map<string,int> & idfInfo,vector<string> & seged)
{
    WPList::iterator it;
    for(it=wordList.begin();it!=wordList.end();it++)
    {
        string word=(*it).word;
        int    idf=(*it).pos;

        map<string,int>::iterator it2;
        it2=idfInfo.find(word);
        if(it2==idfInfo.end())//not found ,adding into
        {
            pair<string,int> value=make_pair(word,idf);
            idfInfo.insert(value);

        }

        seged.push_back(word);
    }


}

void TrainingSetManager::OneIteration(DocsInFeature  & samples,CatCentoriod & sumWeight,CatCentoriod & normWeight)
{
    int wrongCounter=0;
    int totalCounter=0;

    cout<<"samples size="<<samples.size()<<endl;

    /*iterate each document*/
    DocsInFeatureIte it;
    for(it=samples.begin();it!=samples.end();it++,totalCounter++)
    {
        if(totalCounter%50==0)
            cout<<"*&";

        /*cat using current model*/
        int catNum=WhichCatNow(normWeight,(*it).second);

        //cout<<"catNum="<<catNum<<"\tshouldbe num="<<(*it).first<<endl;

        /*tune by dragging and pulling*/
        if(catNum!=(*it).first)//wrong category by current model
        {
            int shouldBeCat=(*it).first;//the right cat
            int indeedCat=catNum;//the current model

            /*drag and pull*/
            PushDragOneDoc(sumWeight,normWeight,(*it).second,shouldBeCat,indeedCat);

            //	cout<<"One PushDragOneDoc"<<endl;
            wrongCounter++;

        }	


    }

    cout<<"wrong counter="<<wrongCounter<<endl;

}

void TrainingSetManager::DragPullCentoriod()
{
    bool bSemantic=false;//if we set the semantic feature


    //	cout<<"now in DragPull Function!"<<endl;

    /*loading the feature files and cat Name ID mapper*/

    if(m_featConvertor.Init(featureFile,bSemantic)==false)
    {
        cout<<"something wrong:we can't load the feature file:"<<featureFile<<"!"<<endl;
        return ;
    }

    //	cout<<"before loading IDMapper!"<<endl;
    m_mapper.LoadCatInfo(catIdMapFile);
    //  cout<<"the catID map file size="<<m_mapper.size();
    //	 cout<<"now after the idmaper's loading"<<endl;

    /*loading the training set*/
    m_trainingSetSeged.clear();
    LoadTrainingSet(dirListFile,m_trainingSetSeged);

    /*pure centoriod method:sum weight and norm weight*/
    DocsInFeature DIF;
    CentoriodSum sumWeight;
    CatCentoriod CC1,CC2;

    Turn2CentoriodFormat(m_trainingSetSeged,DIF,bSemantic);//turn format for furthor process
    cout<<"DIF SIZE="<<DIF.size()<<endl;

    SumCentoriodFeature(DIF,sumWeight);//sum the feathure for each category
    cout<<"sumWeight size="<<sumWeight.size()<<endl;

    TurnSimpleForm(sumWeight,CC1);//turn the format
    cout<<"CC1 SIZE="<<CC1.size()<<endl;

    NormSimpleForm(CC1,CC2);//NORM,here CC1 is the sum weight and CC2 is the norm weight
    cout<<"CC2 SIZE="<<CC2.size()<<endl;

    /*drag and pull*/
    int MAX_IT_TIME=m_maxItTime;//the times to iterate
    int counter=0;
    while(counter++<MAX_IT_TIME)
    {
        OneIteration(DIF,CC1,CC2);//one time
        cout<<"After one iteration!"<<endl;
    }

    WriteCentoriod(CC2,m_centoriodFile);



}

int TrainingSetManager::WhichCatNow(CatCentoriod & normWeight,FeatureVec & document)
{
    /*iterate each cat*/
    CatCentoriodIte it;
    float Max_Sim=0.0;
    int  Max_Cat=-1;

    //cout<<"normWeight size="<<normWeight.size();

    for(it=normWeight.begin();it!=normWeight.end();it++)
    {
        float score=Similarity2(document,(*it).second);
        //	cout<<"cat="<<(*it).first<<"\tscore="<<score<<endl;

        if(score>Max_Sim)//get the better class
        {
            Max_Sim=score;
            Max_Cat=(*it).first;

            //	cout<<"cat="<<Max_Cat<<"\t score="<<Max_Sim<<endl;
        }	

    }


    /*is this faster?*/
    /*int CentorNum=normWeight.size();
      int counter=0;
      for(;counter<CentorNum;counter++)
      {
      float score=Similarity2(document,normWeight);
      if(score>Max_Sim)//get the better class                                   
      {                                                                         
      Max_Sim=score;                                                        
      Max_Cat=counter;
      }	
      }

     */
    return Max_Cat;
}

float TrainingSetManager::Similarity2(FeatureVec & document,FeatureVec & centoriod)
{

    float sim=0.0;

    int currentPos=0;
    int size=centoriod.size();

    FeatVecIte it;
    for(it=document.begin();it!=document.end();it++)
    {
        for(;currentPos<size;currentPos++)
        {
            if(centoriod[currentPos].index==(*it).index)//same feature
            {
                sim+=(*it).weight*(centoriod[currentPos].weight);
                currentPos++;
                break;//go to for
            }

            if(centoriod[currentPos].index>(*it).index)//not found in centoriod
            {
                break;//go to for
            }
        }

    }

    return sim;
}

/*too slow!*/
float TrainingSetManager::Similarity(FeatureVec & document,FeatureVec & centoriod)
{
    float similarity=0.0;

    map<int,float> cache;//temp variable for score computing,int:feature index;float:feature production
    map<int,float> result;

    /*first,put the centoriod features into map*/
    FeatVecIte it;
    for(it=centoriod.begin();it!=centoriod.end();it++)
    {
        pair<int,float> value=make_pair((*it).index,(*it).weight);
        cache.insert(value);
    }

    //	cout<<"centoriod featrue size="<<cache.size()<<"\t and document feature size="<<document.size()<<endl;

    /*iterate the each feature of document,Note:the document contain less feature which is not Zero,so it's faster to iterate document instead of centoriod*/
    map<int,float>::iterator it2;
    for(it=document.begin();it!=document.end();it++)
    {
        int featureIndex=(*it).index;
        it2=cache.find(featureIndex);
        if(it2!=cache.end())//has same feature which is not Zero
        {
            //(*it2).second=(*it2).second*(*it).weight;//update by production value
            float weight=(*it2).second*(*it).weight;//production of 2 weight
            pair<int,float> value=make_pair(featureIndex,weight);
            result.insert(value);

        }


    }

    /*adding the values*/
    for(it2=result.begin();it2!=result.end();it2++)
    {
        similarity+=(*it2).second;	

    }

    return similarity;



}

void TrainingSetManager::PushDragOneDoc(CatCentoriod & sumWeight,CatCentoriod & normWeight,FeatureVec & document,int shouldBeCat,int indeedCat)
{
    /*push shouldbe class's centoriod to document*/
    CatCentoriodIte it;
    it=sumWeight.find(shouldBeCat);
    if(it!=sumWeight.end())
    {
        AddingFeature((*it).second,document);//adding to centoriod
        FeatureVec newFV;
        m_featConvertor.NormorizeFeature((*it).second,newFV);//norm feature
        UpdateNormWeight(normWeight,shouldBeCat,newFV);//update norm weight	

    }


    /*drag indeedCat class's centoriod from document*/
    it=sumWeight.find(indeedCat);
    if(it!=sumWeight.end())
    {
        ReducingFeature((*it).second,document);//reducing to centoriod
        FeatureVec newFV;
        m_featConvertor.NormorizeFeature((*it).second,newFV);//norm feature
        UpdateNormWeight(normWeight,indeedCat,newFV);//update norm weight

    }

}

void TrainingSetManager::UpdateNormWeight(CatCentoriod & normWeight,int catIndex,FeatureVec & newFeatureVec)
{
    CatCentoriodIte it;
    it=normWeight.find(catIndex);
    if(it!=normWeight.end())
    {
        (*it).second=newFeatureVec;//change

    }

}

void TrainingSetManager::ChangeFeature(FeatureVec & centoriod,FeatureVec & document,changeType & type)
{
    map<int,float> cache;//int:feature index;float:weight

    /*adding the centoriod feature into cache*/
    FeatVecIte it;
    for(it=centoriod.begin();it!=centoriod.end();it++)
    {
        pair<int,float> value=make_pair((*it).index,(*it).weight);
        cache.insert(value);

    }

    /*change by featrue weight*/
    map<int,float>::iterator it2;

    for(it=document.begin();it!=document.end();it++)
    {
        it2=cache.find((*it).index);
        if(it2!=cache.end())
        {
            if(type==ADD)//ADDing
                (*it2).second+=m_dragPara*(*it).weight;//adding into centoriod
            else//reducing
            {
                float score=(*it2).second-m_pushPara*(*it).weight;
                if(score<0.0)//mimim is Zero
                    score=0.0;

                (*it2).second=score;//reducing into centoriod
            }

        }


    }

    /*turn into standard format*/
    FeatureVec newFV;
    for(it2=cache.begin();it2!=cache.end();it2++)
    {
        FeatureItem item;
        item.index=(*it2).first;
        item.weight=(*it2).second;
        if(item.weight>0.0)//to filter the ones which equal Zero by reducing process
        {
            newFV.push_back(item);
        }

    }

    /*change the older one by new centriod*/
    centoriod=newFV;
}


void TrainingSetManager::AddingFeature(FeatureVec & centoriod,FeatureVec & document)
{
    changeType type=ADD;
    ChangeFeature(centoriod,document,type);
}

void TrainingSetManager::ReducingFeature(FeatureVec & centoriod,FeatureVec & document)
{
    changeType type=REDUCE;
    ChangeFeature(centoriod,document,type);

}

void TrainingSetManager::Turn2CentoriodFormat(vector<TrainingDocSeged> & trainingSetSeged,DocsInFeature & newFormat,bool bSemantic)
{
    vector<TrainingDocSeged>::iterator it;
    for(it=trainingSetSeged.begin();it!=trainingSetSeged.end();it++)
    {
        int catID=m_mapper.Name2ID((*it).leafCatName);//cat id
        FeatureVec fv;

        if(bSemantic)//semantic feature
        {
            m_featConvertor.ConvertNormSemantic((*it).wordList,fv);//convert and norm

        }
        else//normal
        {
            m_featConvertor.ConvertNorm((*it).wordList,fv);//convert and norm
        }

        pair<int,FeatureVec> value=make_pair(catID,fv);
        newFormat.insert(value);

    }

}

void TrainingSetManager::SumCentoriodFeature(DocsInFeature & newFormat,CentoriodSum & sumValue)
{
    /*iterate each document,adding the features to sumValue according to the catID*/
    DocsInFeatureIte it;
    for(it=newFormat.begin();it!=newFormat.end();it++)
    {
        int catID=(*it).first;
        CentoriodSumIte it2;
        it2=sumValue.find(catID);
        if(it2!=sumValue.end())//find,update parametor
        {
            /*for each feature of the document*/
            FeatVecIte it3;
            for(it3=(*it).second.begin();it3!=(*it).second.end();it3++)
            {
                map<int,float>::iterator it4;
                it4=(*it2).second.find((*it3).index);//whether we have this feature which is not Zero
                /*found the feature ,updating*/
                if(it4!=(*it2).second.end())
                {
                    (*it4).second+=(*it3).weight;//adding feature weight


                }
                else//not found,adding 
                {
                    pair<int,float> value=make_pair((*it3).index,(*it3).weight);
                    (*it2).second.insert(value);

                }

            }


        }
        else//not found ,adding
        {
            map<int,float> cache;//temp store

            /*for each feature of the document*/
            FeatVecIte it3;                                                       
            for(it3=(*it).second.begin();it3!=(*it).second.end();it3++) 
            {
                pair<int,float> value=make_pair((*it3).index,(*it3).weight);  
                cache.insert(value);  
            }	

            /*add into bigger structure*/
            pair<int,map<int,float> > value2=make_pair(catID,cache);
            sumValue.insert(value2);

        }
    }

}

void TrainingSetManager::TurnSimpleForm(CentoriodSum & sumValue,CatCentoriod & simpleCencoriod)
{
    CentoriodSumIte it;
    for(it=sumValue.begin();it!=sumValue.end();it++)
    {
        int catID=(*it).first;
        FeatureVec fv;
        Convert2Feathure((*it).second,fv);//convert from map to feature vector

        pair<int,FeatureVec> value=make_pair(catID,fv);
        simpleCencoriod.insert(value);

    }

}

void TrainingSetManager::NormSimpleForm(CatCentoriod & simpleCencoriod1,CatCentoriod & simpleCencoriod2)
{
    /*iterate each cat*/
    CatCentoriodIte it;
    for(it=simpleCencoriod1.begin();it!=simpleCencoriod1.end();it++)
    {
        FeatureVec fv;
        m_featConvertor.NormorizeFeature((*it).second,fv);//norm
        int catID=(*it).first;

        pair<int,FeatureVec> value=make_pair(catID,fv);
        simpleCencoriod2.insert(value);
    }


}

void TrainingSetManager::Convert2Feathure(map<int,float> & featureSet,FeatureVec & fv)
{
    map<int,float>::iterator it;
    for(it=featureSet.begin();it!=featureSet.end();it++)
    {
        FeatureItem item;
        item.index=(*it).first;
        item.weight=(*it).second;

        fv.push_back(item);
    }

}

void TrainingSetManager::WriteCentoriod(CatCentoriod & normWeigh,string & fileName)
{

    ofstream saveFile(fileName.c_str());

    CatCentoriodIte it;
    for(it=normWeigh.begin();it!=normWeigh.end();it++)
    {
        saveFile<<(*it).first<<"|";
        FeatVecIte it2;

        /*if it's null for a category:because no training set*/
        int TrainingSize=(*it).second.size();
        if(TrainingSize==0)
            saveFile<<"1:0.00001 2:0.00001";//fake data,small enough 

        for(it2=(*it).second.begin();it2!=(*it).second.end();it2++)
        {

            saveFile<<(*it2).index<<":"<<(*it2).weight<<" ";
        }	

        saveFile<<endl;

    }

}
