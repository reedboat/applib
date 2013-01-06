#include"../include/TrainingSetManager.h"

int main()
{
	
	string configFile="config/featureSelection.cfg";

	TrainingSetManager tsm;
	tsm.Init(configFile);


	/*feature selection*/
	tsm.TrainFeature();
	
	 cout<<"Done!"<<endl<<endl;
     

}
