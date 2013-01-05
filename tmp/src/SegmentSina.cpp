/*
*author:junlin zhang
*date:2010-09-13
*/

#include"../include/SegmentSina.h"

segmentSina::segmentSina()
{

}


segmentSina::~segmentSina()
{

}


bool segmentSina::Initial(string & lexiconPath,string & type)
{
    //string lexiconPath="  ";
    lexicon=NULL;
    lexicon = OpenLexicon(lexiconPath.c_str());
    if(lexicon==NULL)
       return false;

    /* …Ë÷√”Ô÷÷ */
    setLexiconWS(lexicon, 0);

    /*type:only segment or "segment and pos"*/
    if(type=="segment")
    {
        SetFlagsOnlySeg();
    }
    else
    {
         SetFlagsSegPos();
    }

    wordseg = InitWordSeg(10000);
    if(wordseg == NULL)
    {
         return false;
    }
      
    return true;

}

bool segmentSina::FreeResource()
{
    if(wordseg != NULL)
    {
        FreeWordSeg(wordseg);
         wordseg = NULL;
    }
       
    if(lexicon != NULL)
    {
       CloseLexicon(lexicon);
       lexicon = NULL;
    }

    return true;

}

void segmentSina::SetFlagsOnlySeg()
{
     flags=0;

     unsigned short segopt;
     unsigned short postagopt;
     unsigned short appopt;
     unsigned short lexiconopt;


     segopt = SEG_OPT_ALL | SEG_OPT_ZUHEQIYI | SEG_OPT_PER | SEG_OPT_SUFFIX;;
     postagopt = 0x7FFF;
     appopt = AOP_TERM_ALL;// | AOP_EXTRACTKEYWORDS_DEFAULT;// | CLASSIFY_USING_KNN;
     lexiconopt = 0x0014;
      
     flags = lexiconopt;
     flags = flags<<16;
    
     flags = flags | appopt;
     flags = flags<<16;
     
     flags = flags | postagopt;
     flags = flags<<16;
      
     flags = flags | segopt;


}


void segmentSina::SetFlagsSegPos()
{
      flags=0;

     unsigned short segopt;
     unsigned short postagopt;
     unsigned short appopt;
     unsigned short lexiconopt;

      segopt = SEG_OPT_ALL | SEG_OPT_ZUHEQIYI | SEG_OPT_PER | SEG_OPT_SUFFIX;;
      postagopt = 0x7FFF;
      appopt = AOP_TERM_ALL;// | AOP_EXTRACTKEYWORDS_DEFAULT;// | CLASSIFY_USING_KNN;
      lexiconopt = 0x0014;
      
      flags = lexiconopt;
      flags = flags<<16;
     
      flags = flags | appopt;
      flags = flags<<16;
      
      flags = flags | postagopt;
      flags = flags<<16;
      
      flags = flags | segopt;


}


bool segmentSina::SegmentOnly(string & sentence,vector<string> & result)
{

       char tmp[1024];

       int iret = AnalyTextWithLex2(lexicon, sentence.c_str(), sentence.length(), "GBK", wordseg, flags , 0);
       if( iret <  0 )
       {
          return false;
       }
     
//       cout<<"word-num="<<wordseg->word_num<<endl;
     
       for (int j = 0; j < wordseg->word_num; j++)
       {
         int lenth = uniToBytes(wordseg->uni+wordseg->wordlist[j].wordPos, wordseg->wordlist[j].wordLen, tmp, 1023, "GBK");
         tmp[lenth] =0;
        
         string tmpstr(tmp);
         result.push_back(tmpstr);

       }

       return true;
}

bool segmentSina::SegmentPos(string & sentence,WPList & result)
{
      char tmp[1024];
    
      int iret = AnalyTextWithLex2(lexicon, sentence.c_str(), sentence.length(), "GBK", wordseg, flags , 0);
      if( iret <  0 )
      {
        return false;
      }
     
     for (int j = 0; j < wordseg->word_num; j++)
     {
         int lenth = uniToBytes(wordseg->uni+wordseg->wordlist[j].wordPos, wordseg->wordlist[j].        wordLen, tmp, 1023, "GBK");
         tmp[lenth] =0;
    
         string tmpstr(tmp);
         int posid=wordseg->wordlist[j].postagid;
         
         WordPos segpos;
         segpos.word=tmpstr;
         segpos.pos=posid;

         result.push_back(segpos);
    
     }
     
    return true;


}


bool segmentSina::SegmentIdf(string & sentence,WPList & result)
{

	 char tmp[1024];
	  
	 int iret = AnalyTextWithLex2(lexicon, sentence.c_str(), sentence.length(), "GBK", wordseg, flags , 0);
	 if( iret <  0 )
	 {
		return false;
	 }
			 
	 for (int j = 0; j < wordseg->word_num; j++)
	 {
		int lenth = uniToBytes(wordseg->uni+wordseg->wordlist[j].wordPos, wordseg->wordlist[j].wordLen, tmp, 1023, "GBK");
		tmp[lenth] =0;
					 
		 string tmpstr(tmp);
		 int posid=wordseg->wordlist[j].idf;
		 if(posid==0)//some new word has the idf value=0,that is wrong,so give a high value
			 posid=70;
					 
		 WordPos segpos;                                                          
		 segpos.word=tmpstr;                                                      
		 segpos.pos=posid;                                                        
					 
		 result.push_back(segpos);                                                
					    
	 }
					    


}



/*something wrong in this kind of reading IDF,don't used it!*/
int segmentSina::WordIDF(char * word)
{
     int iret;
     unsigned long long wordid=10;

	 iret= getWordID(lexicon,word,&wordid);
	 if(iret<0)
	 {
		 return -1;//wrong

	 }

	 int idf=getWordIDF(lexicon,wordid);

	 return idf;

}	
