#include"../include/Classfier.h"

Classfier::Classfier()
{

}

Classfier::~Classfier()
{

}


bool Classfier::Init(IN string & configFile)
{
    /*read config para*/
    m_config.ReadFile(configFile);
    m_segPath      = m_config.Read("segmentPath",m_segPath);
    m_featureFile  = m_config.Read("Features",m_featureFile);
    m_catIdMapFile = m_config.Read("catIdNameMap",m_catIdMapFile);
    m_modelName    = m_config.Read("modelName",m_modelName);
    m_FeatureSize  = m_config.Read("maxFeatureSize",2000);
    m_Semantic     = m_config.Read("semanticOrNot",0);
    float m_catDotProduct = 0.0;

    /*load segmentor*/
    string type="segonly";//"segment";                                                   
    bool bi= m_segmentor.Initial(m_segPath,type);
    if(!bi)
    {
        cout<<"segmentor initial flase"<<endl;
        return false;
    }

    /*load feature */   
    if(m_Semantic==0)
    {	 
        m_featConvertor.Init(m_featureFile,false);                                          
    }
    else
    {
        m_featConvertor.Init(m_featureFile,true);     
    }	 


    /*load cat id map*/                                                           
    if(m_catNameMap.LoadCatInfo(m_catIdMapFile)==false)                           
    {                                                                             
        cout<<"can't load cat id map!"<<endl;                                     
        return false;                                                             
    }                     

    /*load  model*/                                                            
    if(LoadCenteroidVec(m_modelName)==false)                                             
    {                                                                             
        cout<<"can't load Classfier  model!"<<endl;                                      
        return false;                                                             
    }    
}

int Classfier::Predict(const string & content)
{
    //return 1;//for speed test

    string content1=const_cast<string &>(content);

    FeatureVec fv;
    Text2Feature(content1,fv);

    float score;
    //return 1;//for speed test
    return MaxProbCat(fv,m_centoroid,score);

}


float Classfier::Predict(const string & content, OUT string & label)
{
    string content1=const_cast<string &>(content);

    FeatureVec fv;
    Text2Feature(content1,fv);

    float score;

    int catNum=MaxProbCat(fv,m_centoroid,score);
    //double score=(double)score1;
    cout<<"score="<<score<<endl;

    label=CatId2Name(catNum);

    return score;

}

bool Classfier::LoadCenteroidVec(string & fileName)
{
    map<int,vector<float> > cache;


    ifstream fin(fileName.c_str());                                              
    string line;                                                                 
    float featureDotProduct = 0.0;

    while(getline(fin,line))   
    {

        vector<float> oneCentor;
        int catIndex;
        if(ParseLine(line,oneCentor,catIndex,m_FeatureSize,featureDotProduct)==false)
        {
            cout<<"Wrong:parsing Classfier model file("<<fileName<< ") wrong"<<endl;
            return false;
        }
        else
        {
            pair<int,vector<float> > value=make_pair(catIndex,oneCentor);
            cache.insert(value);
        }
        m_centoroidFeatureDotProduct.push_back(sqrt(featureDotProduct));
    }

    m_CatNum=cache.size();

    /*transfer to memory data structure,note:according to the cat index number*/
    map<int,vector<float> >::iterator it;
    for(it=cache.begin();it!=cache.end();it++)
    {
        m_centoroid.push_back((*it).second);

    }


    if(cache.size()!=m_CatNum)
    {
        cout<<"Wrong:the category number is wrong!"<<endl;
        return false;
    }

    return true;
}


bool Classfier::ParseLine(string & Line,vector<float> & centoroid,int & catIndex,int featureSize, float & featureDotProduct)
{
    /*initial centoriod with 0*/
    centoroid.clear();
    int counter=0;
    featureDotProduct = 0.0;
    for(;counter<featureSize;counter++)
    {
        centoroid.push_back(0.0);
    }


    /*parse the line*/
    string seperator="|";
    string seperator2=" ";
    string seperator3=":";

    vector<string> part;
    m_SS.Split(Line,seperator,part);
    if(part.size()!=2)
    {

        cout<<"Format wrong in model file"<<endl;
        cout<<Line<<endl;
        return false;
    }

    string strCatNum=part.at(0);
    int catNum=m_SS.Atoi(strCatNum);
    if(catNum<0)
    {
        cout<<"the cat index <0,wrong!"<<endl;
        return false;
    }

    catIndex=catNum;


    string featurePart=part.at(1);
    vector<string> part2;
    m_SS.Split(featurePart,seperator2,part2);

    vector<string>::iterator it;
    float total=0.0;
    for(it=part2.begin();it!=part2.end();it++)
    {
        vector<string> part3;
        m_SS.Split((*it),seperator3,part3);
        if(part3.size()!=2)
        {
            cout<<"index:value wrong in reading Classfier Model!"<<endl;
            return false;
        }

        string strIndex=part3.at(0);
        string strValue=part3.at(1);

        int index=m_SS.Atoi(strIndex);
        float value=m_SS.Atof(strValue);

        if(index<0)
        {
            cout<<"index:value  index <0 wrong!"<<endl;
            return false;
        }

        if(value<0.0)
        {
            cout<<"index:value value<0 wrong!"<<endl;
            return false;
        }

        centoroid[index]=value;//give the feature weight
        featureDotProduct += value * value;
    }
    return true;

}

void Classfier::Text2Feature(string & content,FeatureVec & feature)
{
    vector<string> seged; 	
    m_segmentor.SegmentOnly(content,seged);  
    m_featConvertor.ConvertNorm(seged,feature);  
}

int Classfier::MaxProbCat(FeatureVec & document,vector<vector<float> > & Centoroid,float & score)
{
    float MaxScoreNow=0.0;
    int   MaxCatIndex=-1;

    int currentCatIndex=0;



    float docDotProduct=0.0;
    FeatVecIte dit;
    for(dit=document.begin();dit!=document.end();dit++)
    {
        docDotProduct+=(*dit).weight*(*dit).weight;	//tip:@zhqm totalDoc +=
    }
    docDotProduct = sqrt(docDotProduct);

    if (docDotProduct == 0){
        score = 0.0;
        return MaxCatIndex;
    }

    vector<vector<float> >::iterator it;
    for(it=Centoroid.begin();it!=Centoroid.end();it++,currentCatIndex++)
    {
        float score=InnerProduct(document,(*it), currentCatIndex);
        score=(float)score/(docDotProduct * m_centoroidFeatureDotProduct.at(currentCatIndex));
        //cout<<"cat="<<currentCatIndex<<"\tscore="<<score<<endl;

        if(score>MaxScoreNow)//a better class
        {
            MaxScoreNow=score;
            MaxCatIndex=currentCatIndex;

        }
    }

    score=MaxScoreNow;
    return MaxCatIndex;
}

string Classfier::CatId2Name(int catId)
{
    return m_catNameMap.ID2Name(catId);

}


/*the document is index:value format because there is less features,centorid is array format for quick access
 **/
float Classfier::InnerProduct(FeatureVec & document,vector<float> & centorid, int catIndex)
{
    /*part of cosine*/
    float similarity=0.0;
    float finalScore=0.0;
    float totalDoc=0.0;

    FeatVecIte it;
    for(it=document.begin();it!=document.end();it++)
    {
        similarity+=((*it).weight*centorid[(*it).index]);//direct access the vector
    }

    //float docLen=inner_product(centorid.begin(),centorid.end(),centorid.begin(),0.0);//tip:可以提前计算好。@zhqm

    return similarity;
}
