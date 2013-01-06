/*
*purpose:C++ Interface for  segment
*author:junlin zhang
*date:2010-09-13
*/

#ifndef SEGMENTSINA_H
#define SEGMENTSINA_H


#include "../../Segment/include/lexicon.h"
#include "../../Segment/include/fromuni.h"
#include "../../Segment/include/touni.h"
#include "../../Segment/include/ustring.h"

#include<string>
#include<vector>
#include<iostream>

using namespace std;

typedef struct _wordPos
{
   string word;
   int    pos;

}WordPos;

typedef vector<WordPos>  WPList;

class segmentSina{
    public:
    segmentSina();
    ~segmentSina();

    bool Initial(string & lexiconPath,string & type);//load resource ans parameter setting; type="segment" or" segpos"
    bool SegmentOnly(string & sentence,vector<string> & result);//only segment a sentence
    bool SegmentPos(string & sentence,WPList & result);//sengment and postag a sentence
    bool SegmentIdf(string & sentence,WPList & result);//segment with idf(the pos to store the idf
    int  WordIDF(char * word);//query idf

	bool FreeResource();//NOTE:DON'T FORGET TO FREE RESOURCE AFTER ALL SEGMENT
   


    protected:
    void SetFlagsOnlySeg();//set the flags to segment only
    void SetFlagsSegPos();//set the flags to segment and postag


    private:
      LEXICON *lexicon;
      WORD_SEGMENT *wordseg;

     unsigned long long flags;


};
#endif
