/*
*author:junlin zhang
*date:2010-09-28
*/

#include"../include/superString.h"


SuperString::SuperString()
{

}


SuperString::~SuperString()
{

}


string SuperString::LTrip(string & str)
{
     string drop=" \n\t\v\r\f";
     return str.erase(0,str.find_first_not_of(drop));

}


string SuperString::RTrip(string & str)
{
     string drop=" \n\r\t\v\f";
     return  str.erase(str.find_last_not_of(drop)+1);

}


string SuperString::Trip(string & str)
{
    string drop=" \n\r\t\v\f";

    str.erase(str.find_last_not_of(drop)+1);
    return str.erase(0,str.find_first_not_of(drop));


}


bool SuperString::StartWith(string & str,string & prefix)
{
       int size1=prefix.length();
       int size2=str.length();

       if(size1>0 && size2>0 && size2>=size1)
       {
           string pre=str.substr(0,size1);
           cout<<"pre="<<pre<<endl;
         
           if(pre==prefix)
           {
              return true;
           }
           else
           {
              return false;
           }
       }
       else //impossible
       {
          return false;
       }
      


}



bool SuperString::EndWith(string & str,string & suffix)
{
       int size1=str.length();
       int size2=suffix.length();

       if(size1>0 && size2>0 && size1>=size2)
       {
            string tmp=str.substr(size1-size2,size2);
            return (tmp==suffix);
       }
       else
       {
            return false;
       }

}


int SuperString::Atoi(string & str)
{
   int num;
   stringstream ss(str);
   ss>>num;
   return num;

}



float SuperString::Atof(string & str)
{
   float num;
   stringstream ss(str);
   ss>>num;
   return num; 

}


string SuperString::ItoS(int num)
{
    stringstream ss;
    ss<<num;
    return ss.str();
}


string SuperString::FtoS(float num) 
{
    stringstream ss;
    ss<<num;
    return ss.str(); 
}

bool SuperString::IsAllEN(string & str)
{
    int length=str.length();

    for(int i=0;i<length;i++)
    {   
        char c=str.at(i);
       // cout<<c<<"\t"<<endl;

        if((c>='a' && c<='z' ||  c>='A' && c<='Z')||c==' '||c>='0' && c<='9')
        {
               
        }
        else
        {

            return false;
        } 
    }

   return true;
}


bool SuperString::IsEnPunct(char ch)
{
      return true;   

}

bool SuperString::containPattern(string & str,string & p1,string & p2)
{
   int pos1=str.find(p1);
   if(pos1!=-1)
   {
      int pos2=str.find(p2,pos1+p1.length());
      if(pos2!=-1)
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
      return false;  
   }
}


string SuperString::deletePatternOnce(string & str,string & p1,string & p2)
{
        int pos1=str.find(p1);
        if(pos1!=-1)
        {
          int pos2=str.find(p2,pos1+p1.length());
          if(pos2!=-1)
          {
             return str.erase(pos1,pos2-pos1+p2.length());
          }
          else    //not found
          {
             return str;
          }
        }
        else     //not found
       {
           return str;  
       }


}

string SuperString::deletePatternAll(string & str,string & p1,string & p2)
{
          string tmpStr=str;

          while(containPattern(tmpStr,p1,p2))
          {
               tmpStr=deletePatternOnce(tmpStr,p1,p2);

          }

          return tmpStr;
}


int SuperString::Count(string & str,string & word)
{
    int pos=str.find(word);

    int count=0;

    while(pos!=-1)
    {
       count++;
       pos=str.find(word,pos+word.length());
    }

   return count;
}


int SuperString::Split(string & str,string & seprator,vector<string> & splited)
{

     int count=0;

     int sepLen=seprator.length();
     if(sepLen==0)
        return 0;

     int senLen=str.length();
     if(senLen==0)
         return 0;

     int start=0;
     int pos=str.find(seprator);
     while(pos!=-1)
     {
         
         string component=str.substr(start,pos-start);
         Trip(component);
         if(component.length()>0)
         {
           splited.push_back(component);
           count++;
         }

         start=pos+sepLen;
         pos=str.find(seprator,pos+sepLen);
        

     }
    
     /*last component*/
     if(start<senLen)
     {
         string component=str.substr(start,senLen-start);
         Trip(component);
         if(component.length()>0)
         {   
             splited.push_back(component);
             count++;
         }
     }    

    return count;

}


bool SuperString::IsDigital(string & str)
{
    int length=str.length();
    
    for(int i=0;i<length;i++)
    {
       char c=str.at(i);
       
        if(!isdigit(c))
        {
            return false;
        } 
            
     }
 
    return true;

}


bool SuperString::IsAlpha(string & str)
{
     int length=str.length();
     
     for(int i=0;i<length;i++)
     {
        char c=str.at(i);
       // cout<<c<<"\t"<<endl;
    
        if((c>='a' && c<='z' ||  c>='A' && c<='Z')||c==' ')
        {
     
        }
        else
        {
    
            return false;
        }
    }
     
    return true;

}


string SuperString::ExtractPatternOnce(string & str,string & p1,string & p2)
{
	int pos1=str.find(p1);
	if(pos1!=-1)
	{
		int pos2=str.find(p2,pos1+p1.length());
		if(pos2!=-1)
		{
			 string targetStr=str.substr(pos1+p1.length(),pos2-pos1-p1.length());
			 return targetStr;
		}
		else    //not found
		{
			return "";
		}
	}
	else     //not found
	{
		return "";
	}


}
