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
	//Aria::init();

	//// Parse Command Line Arguments
	//ArArgumentParser argParser(&argc, argv);
	//argParser.loadDefaultArguments();

	//ArRobot robot;
	//ArPose  pose;

	//// Add Sonars
	//ArSensorReading *sonarSensor[8];

	//int sonarRange[8];

	//ArRobotConnector robotConnector(&argParser, &robot);

	//if (robotConnector.connectRobot())
	//	printf("Robot Connected!\n");

	//robot.runAsync(false);
	//robot.lock(); // Stops the robot from performing actions.
	//robot.enableMotors();
	//robot.unlock(); // All the actions buffered since the robot.lock() instruction will be executed.

	//				// --END ROBOT INITIALIZATION--

	//				// Neural Network Initialization
	//NeuralNetwork neural_network(2, 1, 4, 2);
	//neural_network.initializeWeights("Weights\\FirstTest");

	//vector<double> sensors;
	//vector<double> vel;

	//while (true)
	//{
	//	// Sensor Readings
	//	for (int i = 0; i < 8; i++)
	//	{
	//		sonarSensor[i] = robot.getSonarReading(i);
	//		sonarRange[i] = sonarSensor[i]->getRange();
	//	}
	//	sensors.clear();

	//	sensors.push_back(sonarRange[0] / 6000.0);
	//	sensors.push_back(sonarRange[1] / 6000.0);

	//	vel = neural_network.calcHypothesis(sensors);

	//	printf("Inputs: %f\t-%f\n", sensors[0] * 6000, sensors[1] * 6000);
	//	printf("Outputs: %f\t-%f\n", vel[0] * 300, vel[1] * 300);

	//	robot.setVel2(vel[0] * 300, vel[1] * 300);

	//	ArUtil::sleep(100);
	//}

	//neural_network.deleteNetwork();

	//robot.lock();
	//robot.stop();
	//robot.unlock();

	//Aria::exit();

	//*******************Training*********************

	//Setup the network
	
	NeuralNetworkConfig config;
	NeuralUtility neuralUtility;
	unsigned totalExperiments = 100;
	

	vector<string> m_Constants;
	
	unsigned i = 1;
	
	

		//double alpha = neuralUtility.GenerateRandoms(0, 1);
		//double lambda = neuralUtility.GenerateRandoms(0, 1);
		//double learningRate = neuralUtility.GenerateRandoms(0, 1);

		//string str = to_string(alpha) + ',' + to_string(lambda) + ',' + to_string(learningRate);
		////elements exist
		//if (std::find(m_Constants.begin(), m_Constants.end(), str) != m_Constants.end())
		//{
		//	continue;
		//}
	
		i++;
	
		config.ExperimentNo = 1;
		config.TrainingDataFileName = "M:\\CE889\\Program\\Data\\training_data.csv";
		config.TempFileName = "M:\\CE889\\Program\\Data\\training_data_temp.csv";
		config.ValidationDataFileName = "M:\\CE889\\Program\\Data\\Validation_data.csv";
		config.ResultFileName = "M:\\CE889\\Program\\Results\\ResultFileName_exp.csv";
		config.RMSFileName = "M:\\CE889\\Program\\Results\\RMSFileName_exp.csv";
		config.TotalEpoch = 1000; 
		config.Alpha = 0.005;
		config.Lambda = 1.0;
		config.LearningRate = 0.03;
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



	return 0;
}

