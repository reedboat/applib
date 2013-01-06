/*
 *purpose:X2 to compute the score of one word for featue selection
 *author:junlin zhang
 *date:2011-02-22
 *
 */

#ifndef CHISQUARE_H
#define CHISQUARE_H

#include"math.h"
#include<fstream>
#include<iostream>

using namespace std;

class ChiSquare{

	public:
		ChiSquare();
		~ChiSquare();

		float Compute(float A,float B,float D,float E,float totalDoc,float CatNum);//to compute the chisquare value

	protected:

	private:

};

#endif
