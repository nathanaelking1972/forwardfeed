#pragma once
#include <vector>
using namespace std;
struct NeuralNetworkConfig
{
public:
	double Alpha;
	double LearningRate;
	double Lambda;
	vector<unsigned> Topology;
	char * WeightsFileName;
	char * TrainingDataFileName;
	string ValidationDataFileName;
	string TestDataFileName;
	char * InitialisationFile;
	char * TempFileName;
	char * ResultFileName;
	char * RMSFileName;
	double TotalEpoch;
	double ExperimentNo;
};

