#include"../include/ChiSquare.h"

ChiSquare::ChiSquare()
{

}


ChiSquare::~ChiSquare()
{

}

float ChiSquare::Compute(float A,float B,float D,float E,float totalDoc,float CatNum)
{
	float subscore=(float)((float)totalDoc*(A*E-B*D)*(A*E-B*D)/((A+D)*(B+E)*(A+B)*(D+E)));
	//float CProbability=(float)((float)CatNum/(float)totalDoc);
	
	//cout<<"fenzi="<<totalDoc*(A*E-B*D)*(A*E-B*D)<<"\t fenmu="<<(A+D)*(B+E)*(A+B)*(D+E)<<endl;

	float CProbability=1.0;//not use the class probability
	float score=CProbability*subscore;

	return score;

}
