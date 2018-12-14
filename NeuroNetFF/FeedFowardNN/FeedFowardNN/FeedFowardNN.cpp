// FeedFowardNN.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "NeuralNetwork.h"
#include "NeuralNetworkConfig.h"
#include "NeuralUtility.h"

int main(int argc, char **argv)
{
	// --ROBOT INITIALIZATION--
	// Create Instances
	Aria::init();

	// Parse Command Line Arguments
	ArArgumentParser argParser(&argc, argv);
	argParser.loadDefaultArguments();

	ArRobot robot;
	ArPose  pose;

	// Add Sonars
	ArSensorReading *sonarSensor[8];

	int sonarRange[8];

	ArRobotConnector robotConnector(&argParser, &robot);

	if (robotConnector.connectRobot())
		printf("Robot Connected!\n");

	robot.runAsync(false);
	robot.lock(); // Stops the robot from performing actions.
	robot.enableMotors();
	robot.unlock(); // All the actions buffered since the robot.lock() instruction will be executed.

					// --END ROBOT INITIALIZATION--

	 //initialize and run
	NeuralNetworkConfig config;
	config.InitialisationFile = "c:\\data\\initialisation.csv";
	NeuralNetwork net(config);

	vector<double> input;
	vector<double> result;

	while (true)
	{
		// Sensor Readings
		for (int i = 0; i < 8; i++)
		{
			sonarSensor[i] = robot.getSonarReading(i);
			sonarRange[i] = sonarSensor[i]->getRange();
		}
		input.clear();

		input.push_back(sonarRange[0] / 6000.0);
		input.push_back(sonarRange[1] / 6000.0);

	
		result = net.Run(input);
		
	   
		printf("Inputs: %f\t-%f\n", input[0] * 6000, input[1] * 6000);
		printf("Outputs: %f\t-%f\n", result[0] * 300, result[1] * 300);

		robot.setVel2(result[0] * 300, result[1] * 300);

		ArUtil::sleep(100);
	}

	

	robot.lock();
	robot.stop();
	robot.unlock();

	Aria::exit();


	return 0;
}

void Train()
{
	NeuralNetworkConfig config;
	NeuralUtility neuralUtility;
	unsigned totalExperiments = 100;


	vector<string> m_Constants;

	unsigned i = 1;

	i++;

	config.ExperimentNo = 1;
	//config.TrainingDataFileName = "M:\\CE889\\Program\\Data\\training_data.csv";
	config.TrainingDataFileName = "c:\\Data\\training_data.csv";
	//config.TempFileName = "M:\\CE889\\Program\\Data\\training_data_temp.csv";
	config.TempFileName = "c:\\Data\\training_data_temp.csv";
	//config.ValidationDataFileName = "M:\\CE889\\Program\\Data\\Validation_data.csv";
	config.ValidationDataFileName = "c:\\Data\\Validation_data.csv";
	config.ResultFileName = "c:\\Results\\ResultFileName_exp.csv";
	//config.ResultFileName = "M:\\CE889\\Program\\Results\\ResultFileName_exp.csv";
	config.RMSFileName = "c:\\Results\\RMSFileName_exp.csv";
	//config.RMSFileName = "M:\\CE889\\Program\\Results\\RMSFileName_exp.csv";
	config.TestDataFileName = "c:\\Data\\test_data.csv";
	//config.TestDataFileName = "M:\\CE889\\Program\\Data\\test_data.csv";
	config.TotalEpoch = 1000;
	config.Alpha = 0.040;
	config.Lambda = 1.0;
	config.LearningRate = 0.20;
	config.Topology.push_back(2); //input layer with two neurons
	config.Topology.push_back(4); // hidden layer with 4 neurons
	config.Topology.push_back(2); // output layer with 2 neurons

	NeuralNetwork net(config);
	//initialize the network weights
	//load the input/output file
	//Train the network
	net.StartTraininig();

	//Write the output/errors

	//*****************Validate******************
}
