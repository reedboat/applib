/*
*purpose:the super string to fufill many extra function based on string
*author:junlin zhang
*date:2010-09-28
*/

#ifndef SUPERSTRING_H
#define SUPERSTRING_H

#include<string>
#include<sstream>
#include<iostream>
#include<vector>

using namespace std;

class SuperString{
    public:
       SuperString();
       ~SuperString();

       string LTrip(string & str);  //trip the left side
       string RTrip(string & str); //trip the right side
       string Trip(string & str); //Trip string in both side

       int Count(string & str,string & word);//how many times word in str?
       int Split(string & str,string & seprator,vector<string> & splited);//split the str with seprator


       bool StartWith(string & str,string & prefix);//begin with prefix?
       bool EndWith(string & str,string & suffix); //end with suffix?
	   bool IsDigital(string & str);//all digital?
	   bool IsAlpha(string & str);//all alpha?



       int  Atoi(string & str);     //string to int
       float Atof(string & str);    //string to float
       string ItoS(int num);        //int to string
       string FtoS(float num);      //float to string

       bool IsAllEN(string & str);  //is all the str english char?

	   string ExtractPatternOnce(string & str,string & p1,string & p2);//extract content between p1****p2,no  p tag

	   bool containPattern(string & str,string & p1,string & p2);//contain pattern p1****p2?
       string deletePatternOnce(string & str,string & p1,string & p2);//delete one pattern p1****p2 in str
       string deletePatternAll(string & str,string & p1,string & p2);//delete all pattern substr p1***p2 in str

       string Gbk2Utf(string & gbk);
   
    protected:
       bool IsEnPunct(char ch);//is english punctuation?
        
    private:


};

#endif

