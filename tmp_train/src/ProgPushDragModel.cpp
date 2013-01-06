#include"../include/TrainingSetManager.h"
  
int main()
{
		     
	string configFile="config/dragpush.cfg";
			  
	TrainingSetManager tsm;
	tsm.Init(configFile);

	tsm.DragPullCentoriod();//drag and push centoriod model

}

