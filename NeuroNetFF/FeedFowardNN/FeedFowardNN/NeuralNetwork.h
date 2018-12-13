#pragma once
#include "vector"
#include "NeuralNetworkConfig.h"
#include "Neuron.h"
#include "FileUtility.h"
#include "NeuralUtility.h"
#include "DataRecord.h"
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <assert.h>

using namespace std;

typedef vector <Neuron> Layer;

class NeuralNetwork
{
public:
	NeuralNetwork(const NeuralNetworkConfig &config);
	void StartTraininig();
	
	
	vector<double> Run(const vector<double> &inputs);
	~NeuralNetwork();
private:
	NeuralNetworkConfig _config;
	vector<Layer> m_Layers;
	vector<double> m_Max_Input;
	vector<double> m_Min_Input;
	

	vector<double> m_Max_Output;
	vector<double> m_Min_Output;
	NeuralUtility neuralUtility;
	double m_error;
	vector<double> FeedForward(const vector<double> &input);
	void BackPropogation(const vector<double>  &eExpectedOutput);
	void OutputResults(int epoch);
	void RMSResults(int epoch, double trainingRSM, double validationRSM);
	double ValidationRun();
	
	
};

NeuralNetwork::NeuralNetwork(const NeuralNetworkConfig &config)
{
	//create number of layers of the network
	_config = config;
	unsigned noOfLayers = _config.Topology.size();
	int size = _config.Topology.size();

	for (unsigned i = 0 ; i < noOfLayers; ++i)
	{
		m_Layers.push_back(Layer());
		unsigned noOfOutputs = (i == size - 1  ? 0 : _config.Topology[i + 1]);

		//create neuron per layer including bias
		for (unsigned j = 0; j <= _config.Topology[i]; ++j)
		{
			m_Layers.back().push_back(Neuron(noOfOutputs, j, config));
			
		}
	}

	
	m_Max_Input.push_back(4598.22);
	m_Max_Input.push_back(4989.31);
	m_Min_Input.push_back(274.062);
	m_Min_Input.push_back(677.306);

	m_Max_Output.push_back(186.359);
	m_Max_Output.push_back(300);
	m_Min_Output.push_back(114.47);
	m_Min_Output.push_back(101.751);


}
double  NeuralNetwork::ValidationRun()
{

	double _error = 0;

	ifstream _File(_config.ValidationDataFileName);

	if (_File.eof())
	{
		printf("Failed to load data file!");
		exit;
	}

	string line;
	while (!_File.eof() && getline(_File, line, '\n'))
	{
		stringstream myStream(line);
		string cellData;
		vector <double> data;
		while (getline(myStream, cellData, ','))
		{
			// cellData now has the value of a cell in your csv file.
			data.push_back(stod(cellData));
		}

		vector<double> input;
		input.push_back(data[0]);
		input.push_back(data[1]);

		vector<double> output;
		output.push_back(data[2]);
		output.push_back(data[3]);

		vector<double> result = FeedForward(neuralUtility.Normalise(input, m_Max_Input, m_Min_Input));
		//calculate error

		Layer &lastLayer = m_Layers.back();

		vector<double> normalisedOutput = neuralUtility.Normalise(output, m_Max_Output, m_Min_Output);

		for (unsigned i = 0; i < lastLayer.size() - 1; ++i)
		{
			double deltaError = normalisedOutput[i] - lastLayer[i].GetOutputValue();
			_error = deltaError * deltaError;
		}

		_error = sqrt(_error / (lastLayer.size() - 1)); //RMS Value
		
	}

	//close the file
	_File.close();

	return _error;
}
void NeuralNetwork::StartTraininig()
{

	
	for(unsigned epoch =0;epoch <=_config.TotalEpoch;epoch++)
	{ 
	
		ifstream _File (_config.TrainingDataFileName);
		
		if (_File.eof())
		{
			printf("Failed to load data file!");
			exit;
		}
		
		string line;
		while (!_File.eof() && getline(_File, line, '\n'))
		{
			stringstream myStream(line);
			string cellData;
			vector <double> data;
			while (getline(myStream, cellData, ','))
			{
				// cellData now has the value of a cell in your csv file.
				data.push_back(stod(cellData));
			}

			vector<double> input;
			input.push_back(data[0]);
			input.push_back(data[1]);

			vector<double> output;
			output.push_back(data[2]);
			output.push_back(data[3]);

			vector<double> result = FeedForward(neuralUtility.Normalise(input, m_Max_Input, m_Min_Input));
			//calculate error & backpropogate

			BackPropogation(neuralUtility.Normalise(output, m_Max_Output, m_Min_Output));
			//to record the error

			

		}

		//close the file
		_File.close();

		double errorRMS = ValidationRun();

		OutputResults(epoch);

		RMSResults(epoch, m_error, errorRMS);

		//run the validation data

		
		//record weights and errors
		//calculate mean error

		//resuffle data
		/*if (epoch % 50 == 0 )
			neuralUtility.SuffleData(_config.TrainingDataFileName, _config.TempFileName);*/
	}

}

vector<double> NeuralNetwork::FeedForward(const vector<double> &input)
{
		
	//latch the input into the input neurons
	for (unsigned j = 0; j < input.size(); ++j)
	{
		m_Layers[0][j].SetOutputValue(input[j]);
	}


	//deal with hidden layers

	for (unsigned i = 1; i < m_Layers.size(); ++i)
	{
		Layer &lastLayer = m_Layers[i - 1];
		for (unsigned j = 0; j < m_Layers[i].size() - 1; ++j)
		{
			m_Layers[i][j].FeedForward(lastLayer);
		}
	}

	// output of last layer
	vector<double> returnValues;
	
	Layer lastLayer = m_Layers.back();

	for (unsigned i = 0; i < lastLayer.size()-1; ++i)
	{
		returnValues.push_back(lastLayer[i].GetOutputValue());
	}


	return returnValues;
}

void NeuralNetwork::BackPropogation(const vector<double> &expectedOutput)
{
	
	//calculate the error
	double error = 0;

	Layer &lastLayer = m_Layers.back();

	for (unsigned i = 0; i < lastLayer.size() -1; ++i)
	{
		double deltaError = expectedOutput[i] - lastLayer[i].GetOutputValue();
		error = deltaError * deltaError;
	}
	
	error = sqrt(error / (lastLayer.size() - 1)); //RMS Value

	m_error = error;

	//calculate the gradient for output layer

	for (unsigned i = 0; i < lastLayer.size() - 1; ++i)
	{
		lastLayer[i].CalculateOutputGradients(expectedOutput[i]);
	}

	//calculate the gradient for hidden layer

	for (unsigned i = m_Layers.size() - 2; i > 0; --i)
	{
		Layer &hiddenLayer = m_Layers[i];
		Layer &nextLayer = m_Layers[i + 1];
		for (unsigned j = 0; j < hiddenLayer.size(); ++j)
		{
			hiddenLayer[j].CalculateHiddenGradients(nextLayer);
		}
	}

	//update weights for all layers

	for (unsigned i = m_Layers.size() - 1; i > 0; --i)
	{
		Layer &currentLayer = m_Layers[i];
		Layer &PreviousLayer = m_Layers[i - 1];

		for (unsigned j = 0; j < currentLayer.size() - 1; ++j)
		{
			currentLayer[j].UpdateInputWeights(PreviousLayer);
		}
	}


}

vector<double> NeuralNetwork::Run(const vector<double> &input)
{
	
	//initialize weights
	//normalise weights
	//run forward
	//denormalise data
	return neuralUtility.DeNormalise(FeedForward(neuralUtility.Normalise(input, m_Max_Input, m_Min_Input)), m_Max_Output, m_Min_Output);
}

void NeuralNetwork::OutputResults(int epoch)
{
	
	ofstream out(_config.ResultFileName,std::ofstream::app);

	//write epoh number, alpha, lambda, learningrate
	out << "Experiment " + to_string(_config.ExperimentNo) + "Epoch - " + to_string(epoch) << "lambda - " + to_string(_config.Lambda) << "alpha - " + to_string(_config.Alpha) << "LearningRate - " + to_string(_config.LearningRate) << endl;
	//write error RMS
	out << "RMS - " + to_string(m_error) << endl;
	//weights and deltaweights
	//input to output layer
	for (unsigned i = 0; i < m_Layers.size() - 1; ++i)
	{
		Layer &layer = m_Layers[i];
		for (unsigned j = 0; j < m_Layers.size(); ++j)
		{
			out << layer[j].GetOutputValue()<< ' ';
		}
		out << endl;
		for (unsigned j = 0; j < m_Layers.size(); ++j)
		{
			vector <double> weights = layer[j].GetWeights();
			for (unsigned k = 0; k < weights.size(); ++k)
			{
				out << weights[k] << ' ';
			}
			out << endl;
		}
		
		for (unsigned j = 0; j < m_Layers.size(); ++j)
		{
			vector <double> deltaWeights= layer[j].GetDeltaWeights();
			for (unsigned k = 0; k < deltaWeights.size(); ++k)
			{
				out << deltaWeights[k] << ' ';
			}
			out << endl;
		}
	}

	out.flush();
	out.close();
}

void NeuralNetwork::RMSResults(int epoch, double trainingRSM, double validationRSM  )
{
	ofstream out(_config.RMSFileName, std::ofstream::app);

	out << epoch << "," << trainingRSM  << "," << validationRSM << endl;

	out.flush();
	out.close();
}

NeuralNetwork::~NeuralNetwork()
{
	//realase all variables
}
